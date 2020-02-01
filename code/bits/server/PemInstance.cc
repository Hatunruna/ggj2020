#include "PemInstance.h"

#include <cinttypes>

#include <gf/Log.h>

#include "common/PemProtocol.h"
#include "common/Protocol.h"
#include "Crew.h"

namespace ggj {
  PemInstance::PemInstance(int32_t players)
  : m_players(players)
  , m_deck(players, m_random)
  , m_ship(Ship(m_players))
  {
  }

  void PemInstance::start() {
    assert(getPlayersCount() == m_players);

    auto crew = Crew::createCrew(m_players);
    std::shuffle(crew.begin(), crew.end(), m_random.getEngine());

    auto players = getPlayers();

    for (auto& player : players) {
      PemServerInitRole data;
      data.role = crew.back();
      crew.pop_back();

      switch (data.role) {
        case CrewType::Protector:
          for (auto& card : data.cards) {
            card = m_deck.pickProtectorCard();
          }
          break;

        case CrewType::Rebel:
          for (auto& card : data.cards) {
            card = m_deck.pickRebelCard();
          }
          break;
      }

      Member member;
      member.cards = data.cards;
      m_members.emplace(player.id, std::move(member));

      gf::Log::debug("(PemInstance) Init Role to @%" PRIX64 "\n", player.id);
      send(player.id, data);
    }

    m_votes.clear();
    PemServerStartVoteForCaptain data; // start vote phase
    broadcast(data);

    PemServerStartMoveAndPlay moveAndPlay; // start move and play phase
    broadcast(moveAndPlay);
  }

  bool PemInstance::isFinished() {
    return false;
  }

  void PemInstance::update(ServerPlayer& player, ProtocolBytes& bytes) {
    switch (bytes.getType()) {
      case ClientChatMessage::type: {
        gf::Log::info("(PemInstance) {%" PRIX64 "} Chat message.\n", player.id);
        // deserialize
        auto in = bytes.as<ClientChatMessage>();
        // broadcast the message
        ServerChatMessage out;
        out.message.origin = player.id;
        out.message.author = player.name;
        out.message.content = std::move(in.content);

        if (in.recipient != gf::InvalidId) {
          out.message.recipient = in.recipient;
          send(in.recipient, out);
        } else {
          broadcast(out);
        }

        break;
      }

      case PemClientVoteForCaptain::type: {
        gf::Log::info("(PemInstance) {%" PRIX64 "} Vote for captain.\n", player.id);

        auto in = bytes.as<PemClientVoteForCaptain>();

        auto it = m_members.find(player.id);
        assert(it != m_members.end());

        if (!it->second.voted && it->second.prison == 0) {
          m_votes[in.member]++;
          it->second.voted = true;
        }

        checkEndOfVote(VoteType::Captain);
        break;
      }
      
      case PemClientChoosePrisoner::type: {
        gf::Log::info("(PemInstance) {%" PRIX64 "} Vote for prisoner.\n", player.id);
        auto in = bytes.as<PemClientChoosePrisoner>();

        auto it = m_members.find(player.id);
        assert(it != m_members.end());

        if (!it->second.voted && it->second.prison == 0) {
          auto ut = m_members.find(player.id);
          if(ut->second.captain){
            m_votes[in.member]+= 2;
          }else{
            m_votes[in.member]++;
          }
          it->second.voted = true;
        }

        checkEndOfVote(VoteType::Prison);
        break;
      }

      case PemClientStartMoveAndPlay::type: {
        gf::Log::info("(PemInstance) {%" PRIX64 "} Captain started the round.\n", player.id);
        PemServerStartMoveAndPlay data;
        broadcast(data);
        break;
      }

      case PemClientMoveAndPlay::type: {
        gf::Log::info("(PemInstance) {%" PRIX64 "} Sended his action.\n", player.id);
        auto in = bytes.as<PemClientMoveAndPlay>();
        m_ship.addCrew(in.place, player.id);
        auto it = m_members.find(player.id);
        assert(it != m_members.end());

        it->second.card = in.card;
        it->second.place = in.place;
        checkEndOfTurn();
        break;
      }

    }
  }

  void PemInstance::checkEndOfVote(VoteType type = VoteType::Captain) {
    int32_t electors = 0;

    for (auto& kv : m_members) {
      if (kv.second.prison == 0) {
        ++electors;
      }
    }

    int32_t votes = 0;

    for (auto& kv : m_votes) {
      votes += kv.second;
    }

    if (votes < electors) {
      return;
    }

    std::vector<gf::Id> captains;

    for (auto& kv : m_votes) {
      if (kv.first == gf::InvalidId) {
        continue;
      }

      if (kv.second > votes) {
        captains = { kv.first };
        votes = kv.second;
      } else if (kv.second == votes) {
        captains.push_back(kv.first);
      }
    }

    gf::Id captain = gf::InvalidId;

    if (captains.empty()) {
      std::vector<gf::Id> eligibles;

      for (auto& kv : m_members) {
        if (kv.second.prison == 0) {
          eligibles.push_back(kv.first);
        }
      }

      assert(!eligibles.empty());
      captain = eligibles[m_random.computeUniformInteger<std::size_t>(0, eligibles.size() - 1)];
    } else if (captains.size() == 1) {
      captain = captains.front();
    } else { // draw 
      captain = captains[m_random.computeUniformInteger<std::size_t>(0, captains.size() - 1)];
    }

    m_votes.clear();

    for (auto& kv : m_members) {
      kv.second.voted = false;
    }

    if (type == VoteType::Captain){
      gf::Log::info("(PemInstance) Captain is %" PRIX64 ".\n", captain);
      PemServerChooseCaptain data;
      data.member = captain;
      broadcast(data);
    } else {
      gf::Log::info("(PemInstance) Prisoner is %" PRIX64 ".\n", captain);
      PemServerChoosePrisoner data;
      data.member = captain;
      broadcast(data);
      auto it = m_members.find(captain);
      it->second.prison = 2;
    }

  }


  void PemInstance::checkEndOfTurn(){
    int32_t canPlay = 0; // todo

    m_currentlyPlaying++;

    if ( m_currentlyPlaying < canPlay ) {
      return;
    }

    for( auto &ship : m_ship.places ){
      std::map<CardType,int> cardMap;
      m_action.insert({ ship.first,cardMap });

      for( auto &memberId : ship.second.members ){

        auto it = m_members.find(memberId);
        if ( isPlayable( ship.second.state, it->second.card) ){
          auto at = m_action.find( ship.first );
          auto ut = at->second.find( it->second.card );
          if( ut == at->second.end() ){
            at->second.insert( { it->second.card, 1 } );
          }else{
            ut->second++;
          }
        }else{
          PemServerCardRemoved data;
          send( memberId, data );
        }
      }
    }
    resolution();
  }

  bool PemInstance::isPlayable( PlaceState state,CardType type ){
    switch ( state )
    {
      case PlaceState::Working:{
        if ( type == CardType::Demine || type == CardType::FalseRepair1 || type == CardType::FalseRepair2 || type == CardType::Repair ){
          return false;
        }
        break;
      }
      case PlaceState::Blocked:{
        return false;
      }
      case PlaceState::FalseAlarm:{
        return true;
      }
      case PlaceState::Saboted:{
        if ( type == CardType::Reinforce1 || type == CardType::Reinforce2 || type == CardType::PlaceBomb0 || type == CardType::PlaceBomb1 || type == CardType::PlaceBomb2 ){
          return false;
        }
        break;
      }
      
      case PlaceState::Jammed:{
        return true;
      }

      default:
        break;
    }

    return true;
  }

  void PemInstance::resolution(){
    for(auto &place : m_action){
      auto it = m_ship.places.find(place.first);
      switch (it->second.state){
        case PlaceState::Blocked:{
          // todo resolution
          continue;
          break;
        }
        case PlaceState::Jammed: case PlaceState::Working:{
          
          auto ut = m_action.find(place.first);
          auto at = ut->second.find(CardType::Hide);
          if(at != ut->second.end()){
            for(ssize_t i = 0; i < at->second; i++){
              execute( place.first ,CardType::Hide );
            }
          }
          at = ut->second.find(CardType::Examine);
          if(at != ut->second.end()){
            for(ssize_t i = 0; i < at->second; i++){
              execute( place.first ,CardType::Examine );
            }
          }
          at = ut->second.find(CardType::Reinforce2);
          if(at != ut->second.end()){
            for(ssize_t i = 0; i < at->second; i++){
              execute( place.first ,CardType::Reinforce2 );
            }
          }
          at = ut->second.find(CardType::Reinforce1);
          if(at != ut->second.end()){
            for(ssize_t i = 0; i < at->second; i++){
              execute( place.first ,CardType::Reinforce1 );
            }
          }
          at = ut->second.find(CardType::PlaceBomb0);
          if(at != ut->second.end()){
            for(ssize_t i = 0; i < at->second; i++){
              execute( place.first ,CardType::PlaceBomb0 );
            }
          }
          at = ut->second.find(CardType::PlaceBomb1);
          if(at != ut->second.end()){
            for(ssize_t i = 0; i < at->second; i++){
              execute( place.first ,CardType::PlaceBomb1 );
            }
          }
          at = ut->second.find(CardType::PlaceBomb2);
          if(at != ut->second.end()){
            for(ssize_t i = 0; i < at->second; i++){
              execute( place.first ,CardType::PlaceBomb2 );
            }
          }
          at = ut->second.find(CardType::Track);
          if(at != ut->second.end()){
            for(ssize_t i = 0; i < at->second; i++){
              execute( place.first ,CardType::Track );
            }
          }
          at = ut->second.find(CardType::SetupJammer);
          if(at != ut->second.end()){
            for(ssize_t i = 0; i < at->second; i++){
              execute( place.first ,CardType::SetupJammer );
            }
          }
          at = ut->second.find(CardType::Block);
          if(at != ut->second.end()){
            for(ssize_t i = 0; i < at->second; i++){
              execute( place.first ,CardType::Block );
            }
          }
          at = ut->second.find(CardType::FalseAlarm);
          if(at != ut->second.end()){
            for(ssize_t i = 0; i < at->second; i++){
              execute( place.first ,CardType::FalseAlarm );
            }
          }
          break;
        }

        case PlaceState::FalseAlarm: case PlaceState::Saboted:{
          
          break;
        }
        
        default:
          break;
      }
    }
  }

  void PemInstance::execute(PlaceType place,CardType type){
    switch (type){
      case CardType::Demine:{

        break;
      }
      case CardType::Examine:{
        
        break;
      }
      case CardType::Hide:{
        
        break;
      }
      case CardType::Reinforce1:{
        
        break;
      }
      case CardType::Reinforce2:{
        
        break;
      }
      case CardType::Repair:{
        
        break;
      }
      case CardType::Track:{
        
        break;
      }
      case CardType::Block:{
        
        break;
      }
      case CardType::Release:{
        
        break;
      }
      case CardType::FalseAlarm:{
        
        break;
      }
      case CardType::FalseRepair1:{
        
        break;
      }
      case CardType::FalseRepair2:{
        
        break;
      }
      case CardType::PlaceBomb0:{
        
        break;
      }
      case CardType::PlaceBomb1:{
        
        break;
      }
      case CardType::PlaceBomb2:{
        
        break;
      }
      case CardType::SetupJammer:{
        
        break;
      }
      default:
        break;
    }
  }

}