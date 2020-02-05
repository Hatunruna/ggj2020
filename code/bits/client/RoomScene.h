#ifndef PEM_ROOM_SCENE_H
#define PEM_ROOM_SCENE_H

#include <cstdint>
#include <vector>

#include <gf/ResourceManager.h>
#include <gf/Scene.h>

#include "common/GameSettings.h"
#include "common/ProtocolData.h"

#include "BackgroundEntity.h"
#include "ClientNetwork.h"
#include "LobbyChat.h"

namespace pem {

  struct Scenes;

  class RoomScene : public gf::Scene {
  public:

    RoomScene(Scenes& scenes,  gf::ResourceManager& resources, ClientNetwork& network);

    void startRoom(const GameInstanceSettings& settings);

  private:
    void doProcessEvent(gf::Event& event) override;
    void doUpdate(gf::Time time) override;
    void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;

  private:
    Scenes& m_scenes;
    ClientNetwork& m_network;

    GameInstanceSettings m_settings;
    int32_t m_currentTeam;
    bool m_ready;

    std::vector<PlayerData> m_players;

    LobbyChat m_chat;

    BackgroundEntity m_backgorund;
  };

}

#endif // PEM_ROOM_SCENE_H
