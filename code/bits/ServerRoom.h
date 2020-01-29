#ifndef TMPGAME_SERVER_ROOM_H
#define TMPGAME_SERVER_ROOM_H

#include "GameFactory.h"
#include "GameInstance.h"
#include "ServerGroup.h"
#include "GameSettings.h"

namespace ggj {

  class ServerRoom : public ServerGroup {
  public:
    ServerRoom(GameFactory& factory);

    void update(ServerPlayer& player, ProtocolBytes& bytes) override;

    gf::Id id;
    std::string name;
    GameInstanceSettings settings;

    void broadcastPlayers();

    bool isGameStarted();

  private:
    void startGameIfReady();

  private:
    void doAddPlayer(ServerPlayer& player) override;
    void doRemovePlayer(ServerPlayer& player) override;

  private:
    gf::Ref<GameFactory> m_factory;
    std::unique_ptr<GameInstance> m_instance;
  };

}


#endif // TMPGAME_SERVER_ROOM_H
