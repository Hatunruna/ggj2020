#ifndef PEM_SERVER_LOBBY_H
#define PEM_SERVER_LOBBY_H

#include <gf/Random.h>
#include <gf/Ref.h>

#include "common/ProtocolData.h"

#include "GameFactory.h"
#include "ServerGroup.h"
#include "ServerRoom.h"

namespace pem {

  class ServerLobby : public ServerGroup {
  public:
    ServerLobby(gf::Random& random, GameFactory& factory, const GameSettings& settings);

    void update(ServerPlayer& player, gf::Packet& packet) override;

    std::vector<RoomData> getRooms();

    void broadcastRooms();
    void broadcastPlayers();

  private:
    void doAddPlayer(ServerPlayer& player) override;
    void doRemovePlayer(ServerPlayer& player) override;

    void checkEmptyRoom(ServerRoom& room);

  private:
    gf::Ref<gf::Random> m_random;
    gf::Ref<GameFactory> m_factory;
    GameSettings m_settings;
    std::map<gf::Id, ServerRoom> m_rooms;
  };

}

#endif // PEM_SERVER_LOBBY_H
