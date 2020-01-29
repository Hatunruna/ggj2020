#ifndef TMPGAME_SERVER_PLAYER_H
#define TMPGAME_SERVER_PLAYER_H

#include <cstdint>
#include <string>

#include <gf/TcpSocket.h>

#include "../common/ProtocolBytes.h"

namespace ggj {

  class ServerRoom;

  struct ServerPlayer {
    gf::Id id = gf::InvalidId;
    std::string name;
    gf::TcpSocket socket;
    ServerRoom *room = nullptr;
    int32_t team = -1;
    bool ready = false;

    template<typename T>
    void send(const T& data) {
      ProtocolBytes bytes;
      bytes.is(data);
      socket.sendPacket(bytes.packet);
    }
  };

}

#endif // TMPGAME_SERVER_PLAYER_H
