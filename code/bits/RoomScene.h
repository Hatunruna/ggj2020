#ifndef TMPGAME_ROOM_SCENE_H
#define TMPGAME_ROOM_SCENE_H

#include <cstdint>
#include <vector>

#include <gf/ResourceManager.h>
#include <gf/Scene.h>

#include "Chat.h"
#include "ClientNetwork.h"
#include "GameSettings.h"
#include "ProtocolData.h"

namespace ggj {

  struct Scenes;

  class RoomScene : public gf::Scene {
  public:

    RoomScene(Scenes& scenes, ClientNetwork& network);

    void startRoom(const GameInstanceSettings& settings);

  private:
    void doProcessEvent(gf::Event& event) override;
    void doUpdate(gf::Time time) override;
    void doRender(gf::RenderTarget& target) override;

  private:
    Scenes& m_scenes;
    ClientNetwork& m_network;

    GameInstanceSettings m_settings;
    int32_t m_currentTeam;
    bool m_ready;

    std::vector<PlayerData> m_players;

    Chat m_chat;
  };

}

#endif // TMPGAME_ROOM_SCENE_H
