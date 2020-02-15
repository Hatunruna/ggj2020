#ifndef PEM_SERVER_GROUP_H
#define PEM_SERVER_GROUP_H

#include <cstdint>
#include <vector>

#include <gf/Ref.h>
#include <gf/Packet.h>

#include "common/ProtocolData.h"

#include "ServerPlayer.h"

namespace pem {

  class ServerGroup {
  public:
    virtual ~ServerGroup();

    void addPlayer(ServerPlayer& player);
    void removePlayer(ServerPlayer& player);
    std::vector<PlayerData> getPlayers();

    bool isEmpty() const {
      return m_players.empty();
    }

    int32_t getPlayersCount() {
      return static_cast<int32_t>(m_players.size());
    }

    int32_t getPlayersCountInTeam(int32_t team);

    bool areAllPlayersReady();

    virtual void update(ServerPlayer& player, gf::Packet& packet) = 0;

    template<typename T>
    void send(gf::Id id, const T& data) {
      gf::Packet packet;
      packet.is(data);

      for (ServerPlayer& player : m_players) {
        if (player.id == id) {
          player.socket.sendPacket(packet);
          return;
        }
      }
    }

    template<typename T>
    void broadcast(const T& data) {
      gf::Packet packet;
      packet.is(data);

      for (ServerPlayer& player : m_players) {
        player.socket.sendPacket(packet);
      }
    }

    void cloneTo(ServerGroup& group);

  private:
    virtual void doAddPlayer(ServerPlayer& player);
    virtual void doRemovePlayer(ServerPlayer& player);

  private:
    std::vector<gf::Ref<ServerPlayer>> m_players;
  };

}

#endif // PEM_SERVER_GROUP_H
