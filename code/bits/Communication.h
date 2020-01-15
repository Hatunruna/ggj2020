#ifndef TMPGAME_COMM_H
#define TMPGAME_COMM_H

#include <gf/Queue.h>
#include <gf/TcpSocket.h>

#include "Protocol.h"

namespace ggj {

  class Communication {
  public:
    Communication(const char *hostname, const char *player);

    void send(const ClientPacket& packet);

  public:
    gf::Queue<ServerPacket> queue;

  private:
    void run();

  private:
    gf::TcpSocket m_socket;
  };

}

#endif // TMPGAME_COMM_H
