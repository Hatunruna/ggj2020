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

      gf::Log::debug("[game] PemServerInitRole to @%" PRIX64 "\n\n", player.id);
      send(player.id, data);
    }

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
        broadcast(out);
        break;
      }
    }
  }

}


