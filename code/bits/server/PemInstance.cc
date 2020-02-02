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
      Member member;
      data.role = crew.back();
      crew.pop_back();

      switch (data.role) {
        case CrewType::Protector:
          for (auto& card : data.cards) {
            card = m_deck.pickProtectorCard();
          }
          member.type = CrewType::Protector;
          break;

        case CrewType::Rebel:
          for (auto& card : data.cards) {
            card = m_deck.pickRebelCard();
          }
          member.type = CrewType::Rebel;
          break;
      }

      member.cards = data.cards;
      m_members.emplace(player.id, std::move(member));

      gf::Log::debug("(PemInstance) Init Role to @%" PRIX64 "\n", player.id);
      send(player.id, data);
    }

    m_votes.clear();
    PemServerStartVoteForCaptain data; // start vote phase
    broadcast(data);

  //  PemServerStartMoveAndPlay moveAndPlay; // start move and play phase
  //  broadcast(moveAndPlay);
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
          send(player.id, out);
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
          if(in.member == gf::InvalidId){
            m_votes[in.member]++;
          }else{
            m_votes[in.member]++;
          }it->second.voted = true;
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
      if(type == VoteType::Prison){
        captain = gf::InvalidId;
        PemServerChoosePrisoner data;
        data.member = captain;
        broadcast(data);
        m_votes.clear();
        return;
      }
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
      if(type == VoteType::Prison){
        captain = gf::InvalidId;
        PemServerChoosePrisoner data;
        data.member = captain;
        broadcast(data);
        m_votes.clear();
        return;
      }else{
        captain = captains[m_random.computeUniformInteger<std::size_t>(0, captains.size() - 1)];
      }
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
    int32_t freemen = 0;

    for (auto& kv : m_members) {
      if (kv.second.released) {
        ++freemen;
      }
    }

    ++m_currentlyPlaying;

    if (m_currentlyPlaying < freemen) {
      return;
    }

    // compute the resolutions

    for (auto & kv : m_ship.places) {
      ShipPlace& place = kv.second;

      if (place.members.empty()) {
        continue;
      }

      if (place.blocked > 0) {
        PemServerResolution data;

        Resolution res;
        res.type = ResolutionType::Block;

        data.conclusion.push_back(res);

        for (auto id : place.members) {
          send(id, data);
        }

        continue;
      }

      // get all the actions for that place

      std::vector<CardType> cards;

      for (auto member : place.members) {
        cards.push_back(m_members[member].card);
      }

      auto has = [&cards](CardType ref) {
        return std::any_of(cards.begin(), cards.end(), [ref](CardType type) { return type == ref; });
      };

      if (has(CardType::Hide) || !place.trackers.empty()) {
        PemServerResolution data;

        Resolution res;
        res.type = ResolutionType::Hide;

        // who is not hidden
        for (auto id : place.members) {
          if (m_members[id].card != CardType::Hide) {
            res.members.push_back(id);
          }
        }

        data.conclusion.push_back(res);

        // send to those that are hidden
        for (auto id : place.members) {
          if (m_members[id].card == CardType::Hide) {
            send(id, data);
          }
        }

        // send to those that have a tracker
        for (auto id : place.trackers) {
          send(id, data);
        }
      }
      
      if (has(CardType::Release) && kv.first == PlaceType::Prison){
        PemServerResolution data;
        Resolution res;
        res.type = ResolutionType::Release;
        data.conclusion.push_back(res);

        for (auto id : place.members) {
          if(m_members[id].place == PlaceType::Prison){
            send(id, data);
            m_members[id].prison = 0;
          }
        }
      }

      place.trackers.clear();

      if (has(CardType::Track)) {
        for (auto id : place.members) {
          if (m_members[id].card == CardType::Track) {
            place.trackers.push_back(id);
          }
        }
      }

      if (place.state == PlaceState::Working) {
        if (has(CardType::Demine)) {
          place.bomb = 0;
        }

        if (has(CardType::Reinforce1)) {
          place.reinforcement = 1;
        }

        if (has(CardType::Reinforce2)) {
          place.reinforcement = 2;
        }

        if (has(CardType::PlaceBomb2)) {
          place.bomb = 3;
        }

        if (has(CardType::PlaceBomb1)) {
          place.bomb = 2;
        }

        if (has(CardType::PlaceBomb0)) {
          place.bomb = 1;
        }

        if (has(CardType::Examine)) {
          PemServerResolution data;

          Resolution res;
          res.type = ResolutionType::Examine;
          res.bomb = (place.bomb > 0);

          data.conclusion.push_back(res);

          for (auto id : place.members) {
            if (m_members[id].card == CardType::Examine) {
              send(id, data);
            }
          }

        }

        if (has(CardType::SetupJammer)) {
          place.jammed = 1;
        }

        // everything else has no effect
      } else {
        assert(place.state == PlaceState::Broken);
        if (has(CardType::Repair)) {
          place.bomb = 0;
        }
        if (has(CardType::FalseRepair1)) {
          place.alarm = 1; 
        }
        if (has(CardType::FalseRepair2)) {
          place.alarm = 2; 
        }

        if (has(CardType::SetupJammer)) {
          place.jammed = 1; 
        }

        // everything else has no effect
      }

      // send update to all client
      PemServerUpdateShip update;
      update.state = m_ship.getState();
      broadcast(update);
      m_ship.clear();

      // draw the next card
      for (auto member : m_members) {
        CardType newCard;
        
        for(uint i = 0 ; i < member.second.cards.size() ; ++i){
          if (member.second.cards[i] == member.second.card){
            if (member.second.type == CrewType::Protector){
              newCard = m_deck.pickProtectorCard();
              member.second.cards[i] = newCard;
            }else{
              assert(member.second.type == CrewType::Rebel);
              newCard = m_deck.pickRebelCard();
              member.second.cards[i] =newCard;
            } 
            break;
          }
        }
        PemServerUpdateHand updateHand;
        updateHand.card = newCard;
        send(member.first,updateHand);
      }

      // start vote for prisoner
      PemServerStartVoteForPrisoner prisoner;
      broadcast(prisoner);
      m_currentlyPlaying = 0;
    }
  }

}
