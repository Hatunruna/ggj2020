#ifndef TMPGAME_CLIENT_NETWORK_H
#define TMPGAME_CLIENT_NETWORK_H

#include <atomic>
#include <mutex>
#include <string>

#include <gf/Queue.h>
#include <gf/TcpSocket.h>

#include "Protocol.h"

namespace ggj {

  class ClientNetwork {
  public:
    ClientNetwork();

    bool isConnecting();
    bool isConnected();
    void connect(const std::string& hostname);
    void send(const ClientPacket& packet);
    void disconnect();

  public:
    gf::Queue<ServerPacket> queue;

  private:
    void run(std::string hostname);

  private:
    gf::TcpSocket m_socket;
    std::mutex m_mutex;
    std::atomic_bool m_connecting;
  };

}

#endif // TMPGAME_CLIENT_NETWORK_H