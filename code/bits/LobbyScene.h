#ifndef TMPGAME_LOBBY_SCENE_H
#define TMPGAME_LOBBY_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>
#include <gf/UI.h>

#include "ProtocolData.h"
#include "ClientNetwork.h"

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
    gf::UICharBuffer m_lineBuffer;
    gf::UICharBuffer m_nameBuffer;

    std::vector<PlayerData> m_players;
    std::vector<RoomData> m_rooms;
    std::vector<MessageData> m_messages;
    std::size_t m_selectedRoom;
  };

}

#endif // TMPGAME_LOBBY_SCENE_H
