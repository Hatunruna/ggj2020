#include "Communication.h"

#include <cstring>
#include <stdexcept>
#include <thread>

#include <gf/Log.h>
#include <gf/SerializationOps.h>

namespace ggj {

  Communication::Communication(const std::string& hostname, const std::string& player)
  : m_socket(hostname, Service)
  {
    if (!m_socket) {
      throw std::runtime_error("Connection error.");
    }

    std::thread thread(&Communication::run, this);
    thread.detach();

    if (player.length() > MaxPlayerNameLength) {
      throw std::runtime_error("Name too long.");
    }

    ClientPacket packet;
    packet.type = ClientPacketType::PlayerName;
    std::copy(player.begin(), player.end(), packet.playerName.name.begin());
    packet.playerName.name[player.length()] = '\0';

    gf::Log::info("\t%i %i %i %i\n", packet.playerName.name[0], packet.playerName.name[1], packet.playerName.name[2], packet.playerName.name[3]);

    send(packet);
  }

  void Communication::send(const ClientPacket& packet) {
    m_socket.sendData(packet);
  }

  void Communication::run() {
    for (;;) {
      ServerPacket packet;

      if (!m_socket.recvData(packet)) {
        gf::Log::error("Error while receiving a packet from server\n");
        break;
      }

      queue.push(packet);
    }
  }

}
