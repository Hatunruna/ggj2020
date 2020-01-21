#include "ServerGroup.h"

namespace ggj {
  ServerGroup::~ServerGroup() = default;

  void ServerGroup::addPlayer(ServerPlayer& player) {
    m_players.push_back(player);
  }

  void ServerGroup::removePlayer(ServerPlayer& player) {
    m_players.erase(std::remove(m_players.begin(), m_players.end(), player), m_players.end());
  }

  void ServerGroup::broadcast(const ProtocolBytes& bytes) {
    assert(bytes.type != gf::InvalidId);

    for (ServerPlayer& player : m_players) {
      player.socket.sendPacket(bytes.packet);
    }
  }

}

