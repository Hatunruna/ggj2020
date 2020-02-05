#ifndef PEM_LOBBY_SCENE_H
#define PEM_LOBBY_SCENE_H

#include <gf/Scene.h>
#include <gf/StaticString.h>

#include "common/GameSettings.h"
#include "common/ProtocolData.h"

#include "BackgroundEntity.h"
#include "ClientNetwork.h"
#include "LobbyChat.h"

namespace pem {

  struct Scenes;

  class LobbyScene : public gf::Scene {
  public:
    LobbyScene(Scenes& scenes, gf::ResourceManager& resources, ClientNetwork& network);

  private:
    void doProcessEvent(gf::Event& event) override;
    void doUpdate(gf::Time time) override;
    void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;

  private:
    Scenes& m_scenes;
    ClientNetwork& m_network;
    gf::StaticString<255> m_roomBuffer;
    gf::StaticString<255> m_nameBuffer;

    GameSettings m_settings;
    GameInstanceSettings m_instance;

    std::vector<PlayerData> m_players;
    std::vector<const char *> m_playersView;
    std::vector<RoomData> m_rooms;
    std::vector<const char *> m_roomsView;
    int m_selectedRoom;
    RoomData m_selectedRoomData;

    LobbyChat m_chat;

    BackgroundEntity m_backgorund;
  };

}

#endif // PEM_LOBBY_SCENE_H
