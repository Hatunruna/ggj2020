#ifndef PEM_SERVER_ROOM_H
#define PEM_SERVER_ROOM_H

#include "common/GameSettings.h"

#include "GameFactory.h"
#include "GameInstance.h"
#include "ServerGroup.h"

namespace pem {

  class ServerRoom : public ServerGroup {
  public:
    ServerRoom(GameFactory& factory);

    void update(ServerPlayer& player, gf::Packet& packet) override;

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


#endif // PEM_SERVER_ROOM_H
