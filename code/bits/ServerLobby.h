#ifndef TMPGAME_SERVER_LOBBY_H
#define TMPGAME_SERVER_LOBBY_H

#include <gf/Random.h>
#include <gf/Ref.h>

#include "ProtocolData.h"
#include "ServerGroup.h"
#include "ServerRoom.h"

namespace ggj {

  class ServerLobby : public ServerGroup {
  public:
    ServerLobby(gf::Random& random);

    void update(ServerPlayer& player, ProtocolBytes& bytes) override;

    std::vector<RoomData> getRooms();

    void broadcastRooms();
    void broadcastPlayers();

  private:
    void doAddPlayer(ServerPlayer& player) override;
    void doRemovePlayer(ServerPlayer& player) override;

  private:
    gf::Ref<gf::Random> m_random;
    std::map<gf::Id, ServerRoom> m_rooms;
  };

}

#endif // TMPGAME_SERVER_LOBBY_H