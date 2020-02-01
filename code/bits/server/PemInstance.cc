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

      gf::Log::debug("(PemInstance) Init Role to @%" PRIX64 "\n\n", player.id);
      send(player.id, data);
    }

    m_votes.clear();
    PemServerStartVoteForCaptain data;
    broadcast(data);
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
        auto in = bytes.as<PemClientVoteForCaptain>();

        auto it = m_members.find(player.id);
        assert(it != m_members.end());

        if (!it->second.voted && it->second.prison == 0) {
          m_votes[in.member]++;
          it->second.voted = true;
        }

        checkEndOfVote();
        break;
      }
      
      case PemClientVoteForPrison::type: {
        auto in = bytes.as<PemClientVoteForPrison>();

        auto it = m_members.find(player.id);
        assert(it != m_members.end());

        if (!it->second.voted && it->second.prison == 0) {
          m_votes[in.member]++;
          it->second.voted = true;
        }

        checkEndOfVote();
        break;
      }
    }
  }

  void PemInstance::checkEndOfVote() {
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
    } else {
      captain = captains[m_random.computeUniformInteger<std::size_t>(0, captains.size() - 1)];
    }

    m_votes.clear();

    for (auto& kv : m_members) {
      kv.second.voted = false;
    }

    PemServerChooseCaptain data;
    data.member = captain;
    broadcast(data);
  }

}


