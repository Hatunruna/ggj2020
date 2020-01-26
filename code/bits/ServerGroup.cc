#include "ServerGroup.h"

namespace ggj {
  ServerGroup::~ServerGroup() = default;

  void ServerGroup::addPlayer(ServerPlayer& player) {
    m_players.push_back(player);
    doAddPlayer(player);
  }

  void ServerGroup::removePlayer(ServerPlayer& player) {
    m_players.erase(std::remove(m_players.begin(), m_players.end(), player), m_players.end());
    doRemovePlayer(player);
  }

  std::vector<PlayerData> ServerGroup::getPlayers() {
    std::vector<PlayerData> list;

    for (const ServerPlayer& player : m_players) {
      if (player.name.empty()) {
        continue;
      }

      PlayerData data;
      data.id = player.id;
      data.name = player.name;
      list.push_back(std::move(data));
    }

    return list;
  }

  void ServerGroup::doAddPlayer(ServerPlayer& player) {
    // nothing by default
  }

  void ServerGroup::doRemovePlayer(ServerPlayer& player) {
    // nothing by default
  }

}

