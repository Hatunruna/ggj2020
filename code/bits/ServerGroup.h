#ifndef TMPGAME_SERVER_GROUP_H
#define TMPGAME_SERVER_GROUP_H

#include <vector>

#include <gf/Ref.h>

#include "ProtocolBytes.h"
#include "ProtocolData.h"
#include "ServerPlayer.h"

namespace ggj {

  class ServerGroup {
  public:
    virtual ~ServerGroup();

    void addPlayer(ServerPlayer& player);
    void removePlayer(ServerPlayer& player);
    std::vector<PlayerData> getPlayers();

    bool isEmpty() const {
      return m_players.empty();
    }

    virtual void update(ServerPlayer& player, ProtocolBytes& bytes) = 0;

    template<typename T>
    void broadcast(const T& data) {
      ProtocolBytes bytes;
      bytes.is(data);

      for (ServerPlayer& player : m_players) {
        player.socket.sendPacket(bytes.packet);
      }
    }

  private:
    virtual void doAddPlayer(ServerPlayer& player);
    virtual void doRemovePlayer(ServerPlayer& player);

  private:
    std::vector<gf::Ref<ServerPlayer>> m_players;
  };

}

#endif // TMPGAME_SERVER_GROUP_H
