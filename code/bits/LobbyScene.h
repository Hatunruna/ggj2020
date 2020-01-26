#ifndef TMPGAME_LOBBY_SCENE_H
#define TMPGAME_LOBBY_SCENE_H

#include <gf/Scene.h>
#include <gf/StaticString.h>

#include "GameSettings.h"
#include "ProtocolData.h"
#include "ClientNetwork.h"

namespace ggj {

  struct Scenes;

  class LobbyScene : public gf::Scene {
  public:

    LobbyScene(Scenes& scenes, ClientNetwork& network);

  private:
    void doProcessEvent(gf::Event& event) override;
    void doUpdate(gf::Time time) override;
    void doRender(gf::RenderTarget& target) override;

  private:
    Scenes& m_scenes;
    ClientNetwork& m_network;
    gf::StaticString<255> m_roomBuffer;
    gf::StaticString<255> m_lineBuffer;
    gf::StaticString<255> m_nameBuffer;

    GameSettings m_settings;
    GameInstanceSettings m_instance;

    std::vector<PlayerData> m_players;
    std::vector<const char *> m_playersView;
    std::vector<RoomData> m_rooms;
    std::vector<const char *> m_roomsView;
    int m_selectedRoom;
    std::string m_selectedRoomName;
    GameInstanceSettings m_selectedRoomSettings;
    std::vector<MessageData> m_messages;
    bool m_autoscroll;
  };

}

#endif // TMPGAME_LOBBY_SCENE_H
