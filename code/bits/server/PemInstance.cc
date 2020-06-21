#include "PemInstance.h"

#include <cinttypes>

#include <gf/Log.h>
#include <gf/Sleep.h>

#include "common/PemProtocol.h"
#include "common/Protocol.h"
#include "Crew.h"

namespace {
  float computeDistance(int32_t players) {
    if (players < 6) {
      return 10.0f;
    }

    return 15.0f;
  }

  int computeTurn(int32_t players) {
    if (players < 6) {
      return 15;
    }

    return 20;
  }
}

namespace pem {
  PemInstance::PemInstance(int32_t players)
  : m_players(players)
  , m_deck(players, m_random)
  , m_ship(Ship(m_players))
  , m_distance(computeDistance(players))
  , m_turn(computeTurn(players)) {
  }

  void PemInstance::start() {
    assert(getPlayersCount() == m_players);

    auto crew = Crew::createCrew(m_players);
    std::shuffle(crew.begin(), crew.end(), m_random.getEngine());

    auto players = getPlayers();
    assert(players.size() == crew.size());

    for (auto& player : players) {
      int cardIndex = 0;
      PemServerInitRole data;
      Member member;
      data.role = crew.back();
      crew.pop_back();

      switch (data.role) {
        case CrewType::Protector:
          for (auto& card : data.cards) {
            assert(cardIndex >= 0 && cardIndex < MaxCards);
            card = m_deck.pickProtectorCard();
            member.cards[cardIndex++] = card;
          }
          member.crewType = CrewType::Protector;
          break;

        case CrewType::Rebel:
          for (auto& card : data.cards) {
            assert(cardIndex >= 0 && cardIndex < MaxCards);
            card = m_deck.pickRebelCard();
            member.cards[cardIndex++] = card;
          }
          member.crewType = CrewType::Rebel;
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

  void PemInstance::update(ServerPlayer& player, gf::Packet& packet) {
    switch (packet.getType()) {
      case ClientChatMessage::type: {
        gf::Log::info("(PemInstance) {%" PRIX64 "} Chat message.\n", player.id);
        // deserialize
        auto in = packet.as<ClientChatMessage>();

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
        auto in = packet.as<PemClientMoveAndPlay>();

        // move player into the room
        // m_ship.addCrew(in.place, player.id);

        auto &member = m_members.at(player.id);
        gf::Log::info("(PemInstance) \tCard: %s | Place: %s\n", cardTypeString(in.card).c_str(), placeTypeString(in.place).c_str());

        // TODO: check if is a valid action

        assert(in.indexCard >= 0 && in.indexCard < MaxCards);
        member.card = in.card;
        member.place = in.place;
        member.cardIndex = in.indexCard;

        checkEndOfTurn();
        break;
      }

      case PemClientChoosePrisoner::type: {
        gf::Log::info("(PemInstance) {%" PRIX64 "} Vote for prisoner.\n", player.id);
        auto in = packet.as<PemClientChoosePrisoner>();

        assert(m_members.count(player.id) == 1);

        if (m_votes.count(player.id) == 1) {
          // TODO: Handle error
          break;
        }

        // TODO: check if the vote is valid

        m_votes.emplace(player.id, in.member);

        checkEndOfVote();
        break;
      }
    }
  }

  void PemInstance::resetTurn() {
    for (auto &member: m_members) {
      member.second.place = PlaceType::None;
      member.second.card = CardType::None;
    }

    updateMission();
  }

  void PemInstance::resetVote() {
    m_votes.clear();
  }

  void PemInstance::checkEndOfTurn() {
    int nbActions = std::count_if(m_members.begin(), m_members.end(), [](const auto &entry) {
      const auto member = entry.second;

      bool hasPlayed = member.card != CardType::None && member.place != PlaceType::None;

      return hasPlayed;
    });

    int excpectedActions = std::count_if(m_members.begin(), m_members.end(), [](const auto &entry) {
      const auto member = entry.second;

      bool hasReleaseCard = false;
      for (const auto &card: member.cards) {
        if (card == CardType::Release) {
          hasReleaseCard = true;
          break;
        }
      }
      bool inJail = member.prison > 0;

      return !inJail || hasReleaseCard;
    });

    gf::Log::debug("(PemInstance) actions played %d\n", nbActions);
    gf::Log::debug("(PemInstance) actions expected %d\n", excpectedActions);

    // If some players are not played
    if (nbActions != excpectedActions) {
      return;
    }

    gf::Log::debug("(PemInstance) All players are played\n");

    // Handle actions which non related to room state

    // Spy room
    for (auto &entry: m_members) {
      auto &member = entry.second;

      // Ignore prisonner
      if (member.prison > 0) {
        continue;
      }

      // When the player want spy a place
      if (member.card == CardType::Hide) {
        gf::Log::debug("(PemInstance) Player %lX spies the place '%s'\n", entry.first, placeTypeString(member.place).c_str());

        for (auto &other: m_members) {
          if (other.first != entry.first && other.second.place == member.place) {
            gf::Log::debug("(PemInstance) Player %lX has '%s' at the place '%s'\n", other.first, cardTypeString(other.second.card).c_str(), placeTypeString(member.place).c_str());

            std::string otherName;
            const auto& serverPlayers = getPlayers();
            for (const auto& serverPlayer: serverPlayers) {
              if (serverPlayer.id == other.first) {
                otherName = serverPlayer.name;
                break;
              }
            }

            MessageData message;
            message.author = "server";
            message.origin = gf::InvalidId;
            message.recipient = other.first;
            message.content = "You see the player '" + otherName + "' '" + cardTypeString(other.second.card) + "' at the place '" + placeTypeString(other.second.place) + "'.";

            ServerChatMessage packet;
            packet.message = message;

            send(entry.first, packet);
          }
        }
      }
    }

    // Remove hide action
    // No in previous loop to permit the hidden player to see each other
    for ( auto &entry: m_members) {
      auto &member = entry.second;

      // Ignore prisonner
      if (member.prison > 0) {
        continue;
      }

      // When the player want spy a place
      if (member.card == CardType::Hide) {
        member.card = CardType::None;
      }
    }

    // Blocked room
    for (const auto &entry: m_members) {
      const auto &member = entry.second;

      // Ignore prisonner
      if (member.prison > 0) {
        continue;
      }

      // When the player want block the place
      if (member.card == CardType::Block) {
        gf::Log::debug("(PemInstance) Player %lX jams the place '%s'\n", entry.first, placeTypeString(member.place).c_str());

        // We remove the other cards
        for (auto &other: m_members) {
          // If not same player at same location with a action card
          if (other.first != entry.first && other.second.place == member.place && other.second.card != CardType::None) {
            other.second.card = CardType::None;
            gf::Log::debug("(PemInstance) Player %lX action has been canceled\n", other.first);

            MessageData message;
            message.author = "server";
            message.origin = gf::InvalidId;
            message.recipient = other.first;
            message.content = "The place " + placeTypeString(member.place) + " has been blocked. Our action has been canceled.";

            ServerChatMessage packet;
            packet.message = message;

            send(other.first, packet);
          }
        }
      }
    }

    // Register all action in ship model
    for (const auto &entry: m_members) {
      const auto &member = entry.second;

      // Skip the removed card
      if (member.card == CardType::None) {
        continue;
      }

      // Skip the turn of prisoner, except for an release card
      if (member.prison > 0 && member.card != CardType::Release) {
        continue;
      }

      // If we release the players
      if (member.place == PlaceType::Prison && member.card == CardType::Release) {
        for (auto &prisonner: m_members) {
          if (prisonner.second.prison > 0) {
            prisonner.second.prison = 0;

            PemServerReleasePrisoner release;
            release.prisoner = prisonner.first;
            release.deliverer = entry.first;

            gf::Log::debug("(PemInstance) Release the player %lX by %lX\n", release.prisoner, release.deliverer);

            send(release.prisoner, release);
          }
        }
      }
      // We ignore wrong release card place
      else if (member.card != CardType::Release) {
        m_ship.addAction(member.place, member.card);
      }
    }

    // Sort action by priority
    m_ship.sortActions();

    // Apply the different pending actions
    m_ship.updateActions();

    // Examine a room
    for (const auto &entry: m_members) {
      const auto &member = entry.second;

      if (member.card == CardType::Examine) {
        MessageData message;
        message.author = "server";
        message.origin = gf::InvalidId;
        message.recipient = entry.first;
        message.content = "You examined the room '" + placeTypeString(member.place) + "':";

        for (auto &string: m_ship.getPlaceStateStrings(member.place)) {
          message.content += "\n" + string;
        }

        ServerChatMessage packet;
        packet.message = message;

        send(entry.first, packet);
      }
    }

    // Send the ship states
    PemServerUpdateShip update;
    update.states = std::move(m_ship.getPublicStates());
    broadcast(update);

    resetVote();

    PemServerStartVoteForPrisoner vote;
    broadcast(vote);
  }

  void PemInstance::checkEndOfVote() {
    unsigned expectedVotes = std::count_if(m_members.begin(), m_members.end(), [](const auto &entry) {
      const auto member = entry.second;

      bool inJail = member.prison > 0;

      return !inJail;
    });

    gf::Log::debug("(PemInstance) Expected vote: %d\n", expectedVotes);
    // If all players have voted
    if (m_votes.size() != expectedVotes) {
      return;
    }

    // Initialise count
    std::map<gf::Id, int> voteCount;
    for (const auto &entry: m_members) {
      voteCount.emplace(entry.first, 0);
    }
    voteCount.emplace(gf::InvalidId, 0);

    for (const auto &vote: m_votes) {
      ++voteCount.at(vote.second);
    }

    auto choosenPrisoner = std::max_element(voteCount.begin(), voteCount.end(), [](const auto &entry1, const auto &entry2) {
      return entry1.second < entry2.second;
    });

    // Send result
    PemServerChoosePrisoner vote;
    vote.member = choosenPrisoner->first;
    broadcast(vote);
    gf::Log::debug("(PemInstance) choosen prisonner %lX\n", vote.member);

    // Update prisoner state
    for (auto &member: m_members) {
      if (member.second.prison == 1) {
        member.second.prison = 0;

        PemServerReleasePrisoner release;
        release.prisoner = member.first;
        release.deliverer = gf::InvalidId;

        gf::Log::debug("(PemInstance) Release the player %lX by %lX\n", release.prisoner, release.deliverer);

        // When it's end of sentence, everbody kown it
        broadcast(release);
      }
      else if (member.second.prison > 1) {
        --member.second.prison;
      }
      gf::Log::debug("Member prison state %d\n", member.second.prison);
      assert(member.second.prison >= 0 && member.second.prison <= MaxSentence);
    }

    // Add new prisoner
    if (vote.member != gf::InvalidId) {
      m_members.at(vote.member).prison = MaxSentence;
    }

    // Increase the turn counter
    --m_turn;

    // Get the new distance
    m_distance -= m_ship.computeDistance();

    // Send new card if needed
    for (auto &member: m_members) {
      if (member.second.card != CardType::None) {
        PemServerUpdateHand hand;
        if(member.second.crewType == CrewType::Rebel) {
          hand.card = m_deck.pickRebelCard();
        }
        else {
          hand.card = m_deck.pickProtectorCard();
        }

        // Update player hand
        assert(member.second.cardIndex >= 0 && member.second.cardIndex < 3);
        member.second.cards[member.second.cardIndex] = hand.card;
        member.second.cardIndex = -1;

        // Send new card
        send(member.first, hand);
      }
    }

    resetTurn();

    // If the game is not finsished
    if (m_distance > 0.0f && m_turn > 0) {
      PemServerStartMoveAndPlay moveAndPlay; // start move and play phase
      broadcast(moveAndPlay);
    }
  }

  void PemInstance::updateMission() {
    gf::Log::debug("(PemInstance) remaining distance %f\n", m_distance);

    PemServerMissionStatus mission;
    mission.distance = m_distance;
    mission.turn = m_turn;
    broadcast(mission);

    if (m_distance <= 0.0f) {
      gf::Log::info("(PemInstance) Protector win\n");
    }

    if (m_turn <= 0) {
      gf::Log::info("(PemInstance) Rebel win\n");
    }
  }

}
