#include "ServerGroup.h"

#include <algorithm>

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
      data.team = player.team;
      data.ready = player.ready;
      list.push_back(std::move(data));
    }

    return list;
  }

  int32_t ServerGroup::getPlayersCountInTeam(int32_t team) {
    return std::count_if(m_players.begin(), m_players.begin(), [team](ServerPlayer& player) { return player.team == team; });
  }

  bool ServerGroup::areAllPlayersReady() {
    return std::all_of(m_players.begin(), m_players.end(), [](ServerPlayer& player) { return player.ready; });
  }


  void ServerGroup::cloneTo(ServerGroup& group) {
    group.m_players.clear();

    for (auto& player : m_players) {
      group.addPlayer(player);
    }
  }

  void ServerGroup::doAddPlayer(ServerPlayer& player) {
    // nothing by default
  }

  void ServerGroup::doRemovePlayer(ServerPlayer& player) {
    // nothing by default
  }

}

