#ifndef PEM_SERVER_NETWORK_H
#define PEM_SERVER_NETWORK_H

#include <atomic>
#include <map>
#include <set>

#include <gf/Random.h>
#include <gf/Ref.h>
#include <gf/SocketSelector.h>
#include <gf/TcpListener.h>

#include "ServerLobby.h"
#include "ServerPlayer.h"
#include "GameFactory.h"

namespace pem {

  class ServerNetwork {
  public:
    ServerNetwork(gf::Random& random, GameFactory& factory, const GameSettings& settings);

    void run();

  private:
    static void signalHandler(int sig);

  private:
    gf::Ref<gf::Random> m_random;
    gf::TcpListener m_listener;
    gf::SocketSelector m_selector;

    std::map<gf::Id, ServerPlayer> m_players;
    ServerLobby m_lobby;

    static std::atomic_bool g_running;
  };

}

#endif // PEM_SERVER_NETWORK_H
