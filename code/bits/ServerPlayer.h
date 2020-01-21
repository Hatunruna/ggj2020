#ifndef TMPGAME_SERVER_PLAYER_H
#define TMPGAME_SERVER_PLAYER_H

#include <string>

#include <gf/TcpSocket.h>

namespace ggj {

  class ServerRoom;

  struct ServerPlayer {
    gf::Id id = gf::InvalidId;
    std::string name;
    gf::TcpSocket socket;
    ServerRoom *room = nullptr;
  };

}

#endif // TMPGAME_SERVER_PLAYER_H
