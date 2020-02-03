#include "PemInstance.h"

#include <cinttypes>

#include <gf/Log.h>
#include <gf/Sleep.h>

#include "common/PemProtocol.h"
#include "common/Protocol.h"
#include "Crew.h"

namespace ggj {
  PemInstance::PemInstance(int32_t players)
  : m_players(players)
  , m_deck(players, m_random)
  , m_ship(Ship(m_players)) {
  }

  void PemInstance::start() {
    assert(getPlayersCount() == m_players);

    auto crew = Crew::createCrew(m_players);
    std::shuffle(crew.begin(), crew.end(), m_random.getEngine());

    auto players = getPlayers();
    assert(players.size() == crew.size());

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
          // member.type = CrewType::Protector;
          break;

        case CrewType::Rebel:
          for (auto& card : data.cards) {
            card = m_deck.pickRebelCard();
          }
          // member.type = CrewType::Rebel;
          break;
      }

      // member.cards = data.cards;
      m_members.emplace(player.id, std::move(member));

      gf::Log::debug("(PemInstance) {%" PRIX64 "} Init Role (%s)\n", player.id, data.role == CrewType::Protector ? "protector" : "rebel");
      send(player.id, data);
    }

    // Initialise turn
    resetTurn();

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

        ServerChatMessage out;
        out.message.origin = player.id;
        out.message.author = player.name;
        out.message.content = std::move(in.content);

        // If it's a private message
        if (in.recipient != gf::InvalidId) {
          out.message.recipient = in.recipient;

          // Send to the recipient
          send(in.recipient, out);

          // Send back to the sender
          send(player.id, out);
        }
        else {
          broadcast(out);
        }

        break;
      }

      case PemClientMoveAndPlay::type: {
        gf::Log::info("(PemInstance) {%" PRIX64 "} Move and play.\n", player.id);
        auto in = bytes.as<PemClientMoveAndPlay>();

        // move player into the room
        // m_ship.addCrew(in.place, player.id);

        auto &member = m_members.at(player.id);
        gf::Log::info("(PemInstance) \tCard: %s | Place: %s\n", cardTypeString(in.card).c_str(), placeTypeString(in.place).c_str());

        // TODO: check if is a valid action

        member.card = in.card;
        member.place = in.place;

        checkEndOfTurn();
        break;
      }

      // case PemClientVoteForCaptain::type: {
      //   gf::Log::info("(PemInstance) {%" PRIX64 "} Vote for captain.\n", player.id);

      //   auto in = bytes.as<PemClientVoteForCaptain>();

      //   auto it = m_members.find(player.id);
      //   assert(it != m_members.end());

      //   if (!it->second.voted && it->second.prison == 0) {
      //     m_votes[in.member]++;
      //     it->second.voted = true;
      //   }

      //   checkEndOfVote(VoteType::Captain);
      //   break;
      // }

      // case PemClientChoosePrisoner::type: {
      //   gf::Log::info("(PemInstance) {%" PRIX64 "} Vote for prisoner.\n", player.id);
      //   auto in = bytes.as<PemClientChoosePrisoner>();

      //   auto it = m_members.find(player.id);
      //   assert(it != m_members.end());

      //   if (!it->second.voted && it->second.prison == 0) {
      //     if(in.member == gf::InvalidId){
      //       m_votes[in.member]++;
      //     }else{
      //       m_votes[in.member]++;
      //     }
      //     it->second.voted = true;
      //   }

      //   checkEndOfVote(VoteType::Prison);
      //   break;
      // }

      // case PemClientStartMoveAndPlay::type: {
      //   gf::Log::info("(PemInstance) {%" PRIX64 "} Start move and play.\n", player.id);
      //   PemServerStartMoveAndPlay data;
      //   broadcast(data);
      //   break;
      // }

    }
  }

  void PemInstance::resetTurn() {
    for (auto &member: m_members) {
      member.second.place = PlaceType::None;
      member.second.card = CardType::None;
    }
  }

  void PemInstance::checkEndOfTurn() {
    int nbActions = std::count_if(m_members.begin(), m_members.end(), [](const auto &entry) {
      const auto member = entry.second;
      return member.card != CardType::None && member.place != PlaceType::None;
    });
    int excpectedActions = m_members.size();

    gf::Log::debug("(PemInstance) actions played %d\n", nbActions);
    gf::Log::debug("(PemInstance) actions expected %d\n", excpectedActions);

    // If some players are not played
    if (nbActions != excpectedActions) {
      return;
    }

    gf::Log::debug("(PemInstance) All players are played\n");

    // Register all action in ship model
    for (const auto &entry: m_members) {
      const auto &member = entry.second;

      m_ship.addAction(member.place, member.card);
    }

    // Sort action by priority
    m_ship.sortActions();

    // Apply the different pending actions
    m_ship.updateActions();

    // Send the ship states
    PemServerUpdateShip update;
    update.states = std::move(m_ship.getPublicStates());
    broadcast(update);

    // Dirty tempo to simulate the prisonner election
    gf::sleep(gf::seconds(10));
    resetTurn();
    PemServerStartMoveAndPlay moveAndPlay; // start move and play phase
    broadcast(moveAndPlay);
  }

  // void PemInstance::checkEndOfVote(VoteType type = VoteType::Captain) {
  //   int32_t electors = 0;

  //   for (auto& kv : m_members) {
  //     if (kv.second.prison == 0) {
  //       ++electors;
  //     }
  //   }

  //   int32_t votes = 0;

  //   for (auto& kv : m_votes) {
  //     votes += kv.second;
  //   }

  //   if (votes < electors) {
  //     return;
  //   }

  //   votes = 0;

  //   std::vector<gf::Id> captains;

  //   for (auto& kv : m_votes) {
  //     if (kv.first == gf::InvalidId) {
  //       gf::Log::debug("CONTINUE \n\n\n\n");
  //       continue;
  //     }

  //     if (kv.second > votes) {
  //       captains = { kv.first };
  //       votes = kv.second;
  //     } else if (kv.second == votes) {
  //       captains.push_back(kv.first);
  //     }
  //   }

  //   gf::Log::debug("SIZE OF CAPTAINS : %zu \n\n\n\n", captains.size());

  //   gf::Id captain = gf::InvalidId;

  //   if (captains.empty()) {
  //     if(type == VoteType::Prison){
  //       gf::Log::debug("(PEM) draw vote for prisoner 1 \n");
  //       captain = gf::InvalidId;
  //       PemServerChoosePrisoner data;
  //       data.member = captain;
  //       broadcast(data);
  //       m_votes.clear();
  //       gf::Log::debug("(PEM) draw vote for prisoner 2 \n");
  //       return;
  //     }
  //     std::vector<gf::Id> eligibles;

  //     for (auto& kv : m_members) {
  //       if (kv.second.prison == 0) {
  //         eligibles.push_back(kv.first);
  //       }
  //     }

  //     assert(!eligibles.empty());
  //     captain = eligibles[m_random.computeUniformInteger<std::size_t>(0, eligibles.size() - 1)];
  //   } else if (captains.size() == 1) {
  //     captain = captains.front();
  //   } else { // draw
  //     if(type == VoteType::Prison){
  //       gf::Log::debug("(PEM) draw vote for prisoner 1 \n");
  //       captain = gf::InvalidId;
  //       PemServerChoosePrisoner data;
  //       data.member = captain;
  //       broadcast(data);
  //       m_votes.clear();
  //       gf::Log::debug("(PEM) draw vote for prisoner 2 \n");
  //       return;
  //     }else{
  //       captain = captains[m_random.computeUniformInteger<std::size_t>(0, captains.size() - 1)];
  //     }
  //   }

  //   for (auto& kv : m_members) {
  //     kv.second.voted = false;
  //   }

  //   if (type == VoteType::Captain){
  //     gf::Log::info("(PemInstance) Captain is %" PRIX64 ".\n", captain);
  //     PemServerChooseCaptain data;
  //     data.member = captain;
  //     broadcast(data);
  //   } else {
  //     gf::Log::info("(PemInstance) Prisoner is %" PRIX64 ".\n", captain);
  //     PemServerChoosePrisoner data;
  //     data.member = captain;
  //     broadcast(data);
  //     auto it = m_members.find(captain);
  //     it->second.prison = 2;
  //   }

  //   m_votes.clear();
  // }


  // void PemInstance::checkEndOfTurn(){
  //   int32_t freemen = 0;

  //   for (auto& kv : m_members) {
  //     if (kv.second.prison == 0) {
  //       ++freemen;
  //     }
  //   }

  //   ++m_currentlyPlaying;

  //   if (m_currentlyPlaying < freemen) {
  //     return;
  //   }

  //   gf::Log::debug("(PemInstance) End of turn, computing the resolution\n");

  //   // compute the resolutions

  //   for (auto & kv : m_ship.places) {
  //     ShipPlace& place = kv.second;

  //     gf::Log::debug("(PemInstance) ##### Place %s\n", placeTypeString(kv.first).c_str());

  //     if (place.members.empty()) {
  //       gf::Log::debug("(PemInstance) Nobody here.\n");
  //       continue;
  //     }

  //     if (place.blocked > 0) {
  //       gf::Log::debug("(PemInstance) Place is blocked.\n");

  //       PemServerResolution data;

  //       Resolution res;
  //       res.type = ResolutionType::Block;

  //       data.conclusion.push_back(res);

  //       for (auto id : place.members) {
  //         send(id, data);
  //       }

  //       place.trackers.clear();
  //       continue;
  //     }

  //     // get all the actions for that place

  //     std::vector<CardType> cards;

  //     for (auto member : place.members) {
  //       cards.push_back(m_members[member].card);
  //     }

  //     auto has = [&cards](CardType ref) {
  //       return std::any_of(cards.begin(), cards.end(), [ref](CardType type) { return type == ref; });
  //     };

  //     if (has(CardType::Hide) || !place.trackers.empty()) {
  //       gf::Log::debug("(PemInstance) At least one person is hiding, or a tracker is here.\n");

  //       PemServerResolution data;

  //       Resolution res;
  //       res.type = ResolutionType::Hide;

  //       // who is not hidden
  //       for (auto id : place.members) {
  //         if (m_members[id].card != CardType::Hide) {
  //           res.members.push_back(id);
  //         }
  //       }

  //       data.conclusion.push_back(res);

  //       // send to those that are hidden
  //       for (auto id : place.members) {
  //         if (m_members[id].card == CardType::Hide) {
  //           send(id, data);
  //         }
  //       }

  //       // send to those that have a tracker
  //       for (auto id : place.trackers) {
  //         send(id, data);
  //       }
  //     }

  //     if (has(CardType::Release) && kv.first == PlaceType::Prison){
  //       gf::Log::debug("(PemInstance) At least one person is releasing prisoners.\n");

  //       PemServerResolution data;
  //       Resolution res;
  //       res.type = ResolutionType::Release;
  //       data.conclusion.push_back(res);

  //       for (auto id : place.members) {
  //         if(m_members[id].place == PlaceType::Prison){
  //           send(id, data);
  //           m_members[id].prison = 0;
  //         }
  //       }
  //     }

  //     place.trackers.clear();

  //     if (has(CardType::Track)) {
  //       gf::Log::debug("(PemInstance) At least one person has set a tracker up.\n");

  //       for (auto id : place.members) {
  //         if (m_members[id].card == CardType::Track) {
  //           place.trackers.push_back(id);
  //         }
  //       }
  //     }

  //     if (has(CardType::Block)) {
  //       gf::Log::debug("(PemInstance) At least one person blocked the place.\n");
  //       place.blocked = 2;
  //     }

  //     if (place.state == PlaceState::Working) {
  //       gf::Log::debug("(PemInstance) -- Place is currently working.\n");

  //       if (has(CardType::Demine)) {
  //         gf::Log::debug("(PemInstance) At least one person demined the place.\n");
  //         place.bomb = 0;
  //       }

  //       if (has(CardType::Reinforce1)) {
  //         gf::Log::debug("(PemInstance) At least one person reinforced the place (+1).\n");
  //         place.reinforcement = 1;
  //       }

  //       if (has(CardType::Reinforce2)) {
  //         gf::Log::debug("(PemInstance) At least one person reinforced the place (+2).\n");
  //         place.reinforcement = 2;
  //       }

  //       if (has(CardType::PlaceBomb2)) {
  //         gf::Log::debug("(PemInstance) At least one person placed a bomb (+2).\n");
  //         place.bomb = 3;
  //       }

  //       if (has(CardType::PlaceBomb1)) {
  //         gf::Log::debug("(PemInstance) At least one person placed a bomb (+1).\n");
  //         place.bomb = 2;
  //       }

  //       if (has(CardType::PlaceBomb0)) {
  //         gf::Log::debug("(PemInstance) At least one person placed a bomb (+0).\n");
  //         place.bomb = 1;
  //       }

  //       if (has(CardType::Examine)) {
  //         gf::Log::debug("(PemInstance) At least one person examined the place.\n");

  //         PemServerResolution data;

  //         Resolution res;
  //         res.type = ResolutionType::Examine;
  //         res.bomb = (place.bomb > 0);

  //         data.conclusion.push_back(res);

  //         for (auto id : place.members) {
  //           if (m_members[id].card == CardType::Examine) {
  //             send(id, data);
  //           }
  //         }

  //       }

  //       if (has(CardType::SetupJammer)) {
  //         gf::Log::debug("(PemInstance) At least one person set a jammer up.\n");
  //         place.jammed = 2;
  //       }

  //       if (has(CardType::FalseAlarm)) {
  //         gf::Log::debug("(PemInstance) At least one person set a false alarm up.\n");
  //         place.alarm = 2;
  //       }

  //       // everything else has no effect
  //     } else {
  //       assert(place.state == PlaceState::Broken);
  //       gf::Log::debug("(PemInstance) -- Place is currently broken.\n");

  //       if (has(CardType::Repair)) {
  //         gf::Log::debug("(PemInstance) At least one person repaired the place.\n");
  //         place.bomb = 0;
  //         place.state = PlaceState::Working;
  //       }

  //       if (has(CardType::FalseRepair1)) {
  //         gf::Log::debug("(PemInstance) At least one person made a false repair (+1).\n");
  //         place.repair = 2;
  //       }

  //       if (has(CardType::FalseRepair2)) {
  //         gf::Log::debug("(PemInstance) At least one person made a false repair (+2).\n");
  //         place.repair = 3;
  //       }

  //       if (has(CardType::SetupJammer)) {
  //         gf::Log::debug("(PemInstance) At least one person set a jammer up.\n");
  //         place.jammed = 2;
  //       }

  //       // everything else has no effect
  //     }

  //     // update state

  //     m_ship.endOfActions();

  //     for (auto & kv : m_ship.places) {
  //       auto & place = kv.second;

  //       if (kv.first == PlaceType::Prison) {
  //         // keep prisoners in prison
  //         for (auto it = place.members.begin(); it != place.members.end(); ) {
  //           gf::Id id = *it;

  //           if (m_members[id].prison > 0) {
  //             --m_members[id].prison;

  //             if (m_members[id].prison == 0) {
  //               m_members[id].released = true;
  //               it = place.members.erase(it);
  //             } else {
  //               ++it;
  //             }
  //           } else {
  //             it = place.members.erase(it);
  //           }
  //         }

  //       } else {
  //         place.members.clear();
  //       }
  //     }

  //     // send update to all client
  //     PemServerUpdateShip update;
  //     update.state = m_ship.getState();
  //     broadcast(update);

  //     // draw the next card
  //     for (auto member : m_members) {
  //       CardType newCard;

  //       for(uint i = 0 ; i < member.second.cards.size() ; ++i){
  //         if (member.second.cards[i] == member.second.card){
  //           if(member.second.prison != 0 ){
  //             continue;
  //           }

  //           if (member.second.type == CrewType::Protector){
  //             newCard = m_deck.pickProtectorCard();
  //             member.second.cards[i] = newCard;
  //           }else{
  //             assert(member.second.type == CrewType::Rebel);
  //             newCard = m_deck.pickRebelCard();
  //             member.second.cards[i] =newCard;
  //           }
  //           break;
  //         }
  //       }
  //       PemServerUpdateHand updateHand;
  //       updateHand.card = newCard;
  //       send(member.first,updateHand);
  //     }

  //     // start vote for prisoner
  //     PemServerStartVoteForPrisoner prisoner;
  //     broadcast(prisoner);
  //     m_currentlyPlaying = 0;
  //   }

  //   turn++;
  //   for(auto place : m_ship.places){
  //     if(place.second.state == PlaceState::Broken){

  //       if(place.first == PlaceType::LeftEngine || place.first == PlaceType::RightEngine || place.first == PlaceType::MidEngine){
  //         notWorking += 3;
  //       }


  //       ++notWorking;
  //     }
  //   }

  //   distance += BasicMoveSpeed / notWorking/5;
  //   if(turn == 10 && distance < ligthyear){
  //     // REBEL WON
  //   }else{
  //     // PROTECTOR WON
  //   }
  //   notWorking = 0;
  // }

  // //  15   ::: 100000

}
