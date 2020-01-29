#include "ClientNetwork.h"

#include <cstring>
#include <stdexcept>
#include <thread>

#include <gf/Log.h>
#include <gf/SerializationOps.h>

#include "common/Protocol.h"

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

  void ClientNetwork::disconnect() {
    ClientDisconnect data;
    send(data);
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
      ProtocolBytes bytes;

      switch (m_socket.recvPacket(bytes.packet)) {
        case gf::SocketStatus::Data:
          queue.push(std::move(bytes));
          break;
        case gf::SocketStatus::Error:
          gf::Log::error("Error while receiving a packet from server\n");
          return;
        case gf::SocketStatus::Close:
          gf::Log::info("End of connection to the server\n");
          return;
        case gf::SocketStatus::Block:
          assert(false);
          break;
      }
    }
  }

}
