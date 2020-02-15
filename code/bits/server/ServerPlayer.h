#ifndef PEM_SERVER_PLAYER_H
#define PEM_SERVER_PLAYER_H

#include <cstdint>
#include <string>

#include <gf/Packet.h>
#include <gf/TcpSocket.h>

namespace pem {

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
      gf::Packet packet;
      packet.is(data);
      socket.sendPacket(packet);
    }
  };

}

#endif // PEM_SERVER_PLAYER_H
