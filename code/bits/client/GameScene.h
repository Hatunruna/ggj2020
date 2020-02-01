#ifndef TMPGAME_GAME_SCENE_H
#define TMPGAME_GAME_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>

#include "ClientNetwork.h"
#include "ShipEntity.h"
#include "PlayerInfo.h"
#include "ViewAdaptator.h"
#include "Chat.h"

namespace ggj {

  struct Scenes;

  class GameScene : public gf::Scene {
  public:
    GameScene(Scenes& scenes, ClientNetwork& network, gf::ResourceManager& resources);

    void initializeHand(CardType cards[MaxCards]);

  private:
    void doHandleActions(gf::Window& window) override;
    void doProcessEvent(gf::Event &event) override;
    void doUpdate(gf::Time time) override;
    void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    Scenes& m_scenes;
    ClientNetwork& m_network;

    gf::Action m_escapeAction;

    ViewAdaptator m_adaptator;

    ShipEntity m_ship;
    PlayerInfo m_info;

    Chat m_chat;
  };

}

#endif // TMPGAME_GAME_SCENE_H
