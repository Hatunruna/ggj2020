#include <cassert>
#include <cstdlib>

#include <gf/Log.h>
#include <gf/TcpListener.h>

#include "bits/Constants.h"
#include "bits/Protocol.h"

#include "config.h"

int main() {
  gf::TcpListener listener(ggj::Service);

  if (!listener) {
    return EXIT_FAILURE;
  }

  gf::Log::info("Server waiting for connections on port %s...\n", ggj::Service);

  std::vector<gf::TcpSocket> sockets;

  for (std::size_t i = 0; i < ggj::PlayersCount; ++i) {
    gf::TcpSocket socket = listener.accept();
    gf::Log::info("Player #%zu connected\n", i);

    sockets.push_back(std::move(socket));
  }

  for (;;) {
    ggj::ClientPacket packet;

    for (auto& socket : sockets) {
      if (!socket.recvData(packet)) {
        // TODO: handle error
        return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}
