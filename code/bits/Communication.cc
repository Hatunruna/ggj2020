#include "Communication.h"

#include <cstring>
#include <stdexcept>
#include <thread>

#include <gf/Log.h>
#include <gf/SerializationOps.h>

namespace ggj {

  Communication::Communication(const char *hostname, const char *player)
  : m_socket(hostname, Service)
  {
    if (!m_socket) {
      throw std::runtime_error("Connection error.");
    }

    std::thread thread(&Communication::run, this);
    thread.detach();

    std::size_t length = std::strlen(player);

    if (length > MaxPlayerNameLength) {
      throw std::runtime_error("Name too long.");
    }

    ClientPacket packet;
    packet.type = ClientPacketType::PlayerName;
    std::copy_n(player, length + 1, packet.playerName.name.begin());

    send(packet);
  }

  void Communication::send(const ClientPacket& packet) {
    m_socket.sendData(packet);
  }

  void Communication::run() {
    try {
      gf::Log::info("Communication thread started!\n");

      for (;;) {
        ServerPacket packet;

        if (!m_socket.recvData(packet)) {
          gf::Log::error("Error while receiving a packet from server\n");
          continue;
        }

        queue.push(packet);
      }
    } catch (std::exception& ex) {
      gf::Log::error("Exception: %s\n", ex.what());
    }
  }


}

