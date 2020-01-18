#ifndef TMPGAME_LOBBY_SCENE_H
#define TMPGAME_LOBBY_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>
#include <gf/UI.h>

#include "ClientNetwork.h"
#include "Protocol.h"

namespace ggj {

  struct Scenes;

  class LobbyScene : public gf::Scene {
  public:

    LobbyScene(Scenes& scenes, ClientNetwork& network, gf::ResourceManager& resources);

  private:
    void doProcessEvent(gf::Event& event) override;
    void doUpdate(gf::Time time) override;
    void doRender(gf::RenderTarget& target) override;


  private:
    Scenes& m_scenes;
    ClientNetwork& m_network;
    gf::UI m_ui;
    gf::UICharBuffer m_roomBuffer;

    std::vector<RoomName> m_rooms;
    std::size_t m_selectedRoom;
  };

}

#endif // TMPGAME_LOBBY_SCENE_H
