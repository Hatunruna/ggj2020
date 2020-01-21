#include "ServerNetwork.h"

#include <csignal>
#include <cinttypes>
#include <stdexcept>

#include <gf/Log.h>

#include "Protocol.h"

namespace ggj {

  ServerNetwork::ServerNetwork(gf::Random& random)
  : m_random(random)
  , m_listener(Service)
  {
    if (!m_listener) {
      throw std::runtime_error("Can not start listener.");
    }

    std::signal(SIGINT, &ServerNetwork::signalHandler);
    m_selector.addSocket(m_listener);

    gf::Log::info("Server waiting for connections on port %s...\n", Service);
  }

  void ServerNetwork::run() {
    static constexpr gf::Time Timeout = gf::seconds(1); // wake up the server every seconds to see if something happened

    while (g_running) {
      auto status = m_selector.wait(Timeout);

      if (status == gf::SocketSelectorStatus::Timeout) {
        continue;
      }

      if (status == gf::SocketSelectorStatus::Error) {
        gf::Log::error("An error occurred in the selector. Stopping the server...\n");
        break;
      }

      std::vector<gf::Id> purgatory;

      for (auto& kv : m_players) {
        auto& player = kv.second;

        if (m_selector.isReady(player.socket)) {
          ProtocolBytes bytes;

          switch (player.socket.recvPacket(bytes.packet)) {
            case gf::SocketStatus::Data:
              switch (bytes.getType()) {
                case ClientDisconnect::type:
                  gf::Log::info("Player #%" PRIX64 " disconnected.\n", player.id);
                  purgatory.push_back(player.id);
                  break;
                default:
                  m_lobby.update(player, bytes);
                  break;
              }
              break;

            case gf::SocketStatus::Error:
              gf::Log::error("Error receiving data from player #%" PRIX64 ".", player.id);
              // fallthrough
            case gf::SocketStatus::Close:
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

        gf::Id id = m_random.computeUniformInteger(std::numeric_limits<gf::Id>::min(), std::numeric_limits<gf::Id>::max()); // assume it's unique

        ServerPlayer player;
        player.id = id;
        player.name = "Player #" + std::to_string(player.id); // assume it's unique
        player.socket = std::move(socket);

        auto res = m_players.emplace(id, std::move(player));
        assert(res.second);
        auto& actualPlayer = res.first->second;
        m_selector.addSocket(actualPlayer.socket);
        m_lobby.addPlayer(actualPlayer);

        gf::Log::info("Player #%" PRIX64 " connected.\n", id);
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
    g_running = false;
  }

  std::atomic_bool ServerNetwork::g_running(true);

}

