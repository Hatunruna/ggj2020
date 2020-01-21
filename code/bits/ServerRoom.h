#ifndef TMPGAME_SERVER_ROOM_H
#define TMPGAME_SERVER_ROOM_H

#include "ServerGroup.h"

namespace ggj {

  class ServerRoom : public ServerGroup {
  public:


    void update(ServerPlayer& player, ProtocolBytes& bytes) override;

    std::string name;
  };

}


#endif // TMPGAME_SERVER_ROOM_H
