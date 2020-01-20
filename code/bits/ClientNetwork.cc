#include "ClientNetwork.h"

#include <cstring>
#include <stdexcept>
#include <thread>

#include <gf/Log.h>
#include <gf/SerializationOps.h>

namespace ggj {

  ClientNetwork::ClientNetwork()
  : m_connecting(false)
  {

  }


  bool ClientNetwork::isConnecting() {
    return m_connecting;
  }

  bool ClientNetwork::isConnected() {
    std::lock_guard<std::mutex> guard(m_mutex);
    return static_cast<bool>(m_socket);
  }

  void ClientNetwork::connect(const std::string& hostname) {
    m_connecting = true;

    std::thread thread(&ClientNetwork::run, this, hostname);
    thread.detach();

  }

  void ClientNetwork::send(const ClientPacket& packet) {
    m_socket.sendData(packet);
  }

  void ClientNetwork::disconnect() {
    ClientPacket packet;
    packet.type = ClientPacketType::Disconnect;
    send(packet);
    m_socket = gf::TcpSocket();
    m_connecting = false;
  }

  void ClientNetwork::run(std::string hostname) {
    gf::TcpSocket socket(hostname, Service);

    if (!socket) {
      m_connecting = false;
      return;
    }

    {
      std::lock_guard<std::mutex> guard(m_mutex);
      m_socket = std::move(socket);
    }

    for (;;) {
      ServerPacket packet;

      if (!m_socket.recvData(packet)) {
        gf::Log::error("Error while receiving a packet from server\n");
        // TODO: push a disconnection packet
        break;
      }

      queue.push(packet);
    }
  }

}
