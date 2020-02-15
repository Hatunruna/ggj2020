#include "ServerNetwork.h"

#include <csignal>
#include <cinttypes>
#include <stdexcept>

#include <gf/Log.h>

#include "common/Protocol.h"

namespace pem {

  ServerNetwork::ServerNetwork(gf::Random& random, GameFactory& factory, const GameSettings& settings)
  : m_random(random)
  , m_listener(Service)
  , m_lobby(random, factory, settings)
  {
    if (!m_listener) {
      throw std::runtime_error("Can not start listener.");
    }

    std::signal(SIGINT, &ServerNetwork::signalHandler);
    m_selector.addSocket(m_listener);

    gf::Log::info("(SERVER) Waiting for connections on port %s...\n", Service);
  }

  void ServerNetwork::run() {
    static constexpr gf::Time Timeout = gf::seconds(1); // wake up the server every seconds to see if something happened

    while (g_running) {
      auto status = m_selector.wait(Timeout);

      if (status == gf::SocketSelectorStatus::Timeout) {
        continue;
      }

      if (status == gf::SocketSelectorStatus::Error) {
        gf::Log::error("(SERVER) An error occurred in the selector. Stopping the server...\n");
        break;
      }

      std::vector<gf::Id> purgatory;

      for (auto& kv : m_players) {
        auto& player = kv.second;

        if (m_selector.isReady(player.socket)) {
          gf::Packet packet;

          switch (player.socket.recvPacket(packet)) {
            case gf::SocketStatus::Data:
              switch (packet.getType()) {
                case ClientDisconnect::type:
                  gf::Log::info("(SERVER) {%" PRIX64 "} Disconnected.\n", player.id);
                  purgatory.push_back(player.id);
                  break;
                default:
                  m_lobby.update(player, packet);
                  break;
              }
              break;

            case gf::SocketStatus::Error:
              gf::Log::error("(SERVER) {%" PRIX64 "} Error receiving data.\n", player.id);
              // fallthrough
            case gf::SocketStatus::Close:
              gf::Log::info("(SERVER) {%" PRIX64 "} Socket closed.\n", player.id);
              purgatory.push_back(player.id);
              break;
            case gf::SocketStatus::Block:
              assert(false);
              break;
          }
        }
      }

      if (m_selector.isReady(m_listener)) {
        gf::TcpSocket socket = m_listener.accept();

        if (!socket) {
          continue;
        }

        gf::Id id = m_random.get().computeId(); // assume it's unique

        ServerPlayer playerInstance;
        playerInstance.id = id;
        playerInstance.name = ""; // no name yet
        playerInstance.socket = std::move(socket);

        auto res = m_players.emplace(id, std::move(playerInstance));
        assert(res.second);
        auto& player = res.first->second;
        m_selector.addSocket(player.socket);
        m_lobby.addPlayer(player);

        gf::Log::info("(SERVER) {%" PRIX64 "} Connected.\n", player.id);
      }

      if (!purgatory.empty()) {
        for (auto id : purgatory) {
          auto it = m_players.find(id);
          assert(it != m_players.end());
          auto& player = it->second;
          m_selector.removeSocket(player.socket);
          m_lobby.removePlayer(player);
          m_players.erase(it);
        }
      }
    }
  }

  void ServerNetwork::signalHandler(int sig) {
    assert(sig == SIGINT);
    g_running = false;
  }

  std::atomic_bool ServerNetwork::g_running(true);

}

