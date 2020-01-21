#ifndef TMPGAME_SERVER_LOBBY_H
#define TMPGAME_SERVER_LOBBY_H

#include "ServerGroup.h"
#include "ServerRoom.h"

namespace ggj {

  class ServerLobby : public ServerGroup {
  public:

    void update(ServerPlayer& player, ProtocolBytes& bytes) override;


  private:
    std::map<gf::Id, ServerRoom> m_rooms;
  };

}

#endif // TMPGAME_SERVER_LOBBY_H
