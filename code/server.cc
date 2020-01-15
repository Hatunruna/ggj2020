#include <cstdlib>

#include <gf/Log.h>
#include <gf/TcpListener.h>

#include "bits/Constants.h"

#include "config.h"

int main() {
  gf::TcpListener listener(ggj::Service);

  if (!listener) {
    return EXIT_FAILURE;
  }

  gf::Log::info("Server waiting for connections on port %s...\n", ggj::Service);

  for (std::size_t i = 0; i < ggj::PlayersCount; ++i) {
    gf::TcpSocket socket = listener.accept();
    gf::Log::info("Player #%zu connected\n", i);


  }

  return EXIT_SUCCESS;
}
