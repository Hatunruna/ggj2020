#ifndef TMPGAME_SERVER_ROOM_H
#define TMPGAME_SERVER_ROOM_H

#include "ServerGroup.h"
#include "GameSettings.h"

namespace ggj {

  class ServerRoom : public ServerGroup {
  public:

    void update(ServerPlayer& player, ProtocolBytes& bytes) override;

    gf::Id id;
    std::string name;
    GameInstanceSettings settings;

    void broadcastPlayers();

  private:
    void doAddPlayer(ServerPlayer& player) override;
    void doRemovePlayer(ServerPlayer& player) override;

  };

}


#endif // TMPGAME_SERVER_ROOM_H
