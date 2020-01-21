#ifndef TMPGAME_SERVER_GROUP_H
#define TMPGAME_SERVER_GROUP_H

#include <vector>

#include <gf/Ref.h>

#include "ProtocolBytes.h"
#include "ServerPlayer.h"

namespace ggj {

  class ServerGroup {
  public:
    virtual ~ServerGroup();

    void addPlayer(ServerPlayer& player);
    void removePlayer(ServerPlayer& player);

    virtual void update(ServerPlayer& player, ProtocolBytes& bytes) = 0;

    void broadcast(const ProtocolBytes& bytes);

  private:
    std::vector<gf::Ref<ServerPlayer>> m_players;
  };


}

#endif // TMPGAME_SERVER_GROUP_H
