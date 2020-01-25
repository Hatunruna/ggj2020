#ifndef TMPGAME_SERVER_PLAYER_H
#define TMPGAME_SERVER_PLAYER_H

#include <string>

#include <gf/TcpSocket.h>

#include "ProtocolBytes.h"

namespace ggj {

  class ServerRoom;

  struct ServerPlayer {
    gf::Id id = gf::InvalidId;
    std::string name;
    gf::TcpSocket socket;
    ServerRoom *room = nullptr;

    template<typename T>
    void send(const T& data) {
      ProtocolBytes bytes;
      bytes.is(data);
      socket.sendPacket(bytes.packet);
    }
  };

}

#endif // TMPGAME_SERVER_PLAYER_H
