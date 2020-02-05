#ifndef PEM_CLIENT_NETWORK_H
#define PEM_CLIENT_NETWORK_H

#include <atomic>
#include <mutex>
#include <string>

#include <gf/Queue.h>
#include <gf/TcpSocket.h>

#include "common/ProtocolBytes.h"

namespace pem {

  class ClientNetwork {
  public:
    ClientNetwork();

    bool isConnecting();
    bool isConnected();
    void connect(const std::string& hostname);
    void disconnect();

    template<typename T>
    void send(const T& data) {
      ProtocolBytes bytes;
      bytes.is(data);
      m_socket.sendPacket(bytes.packet);
    }

  public:
    gf::Queue<ProtocolBytes> queue;

  private:
    void run(std::string hostname);

  private:
    gf::TcpSocket m_socket;
    std::mutex m_mutex;
    std::atomic_bool m_connecting;
  };

}

#endif // PEM_CLIENT_NETWORK_H
