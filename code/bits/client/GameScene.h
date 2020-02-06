#ifndef PEM_GAME_SCENE_H
#define PEM_GAME_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>
#include <gf/Widgets.h>

#include <SFML/Audio.hpp>

#include "ClientNetwork.h"
#include "GameChat.h"
#include "GameModel.h"
#include "PlayerInfo.h"
#include "ShipEntity.h"
#include "StarsEntity.h"
#include "ViewAdaptator.h"
#include "VoteEntity.h"

namespace pem {

  struct Scenes;

  class GameScene : public gf::Scene {
  public:
    GameScene(Scenes& scenes, ClientNetwork& network, gf::ResourceManager& resources);

    void initialize(const std::vector<PlayerData> &players);
    void initializeHand(CardType cards[MaxCards]);

  private:
    void doHandleActions(gf::Window& window) override;
    bool doEarlyProcessEvent(gf::Event &event) override;
    void doProcessEvent(gf::Event &event) override;
    void doUpdate(gf::Time time) override;
    void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    void resetGameModel();
    void resetTurn();

  private:
    Scenes& m_scenes;
    ClientNetwork& m_network;

    gf::Action m_escapeAction;

    GameModel m_model;

    StarsEntity m_stars;
    ShipEntity m_ship;
    PlayerInfo m_info;
    GameChat m_chat;
    VoteEntity m_vote;

    ViewAdaptator m_adaptator;

    sf::Sound m_fx;
    sf::Sound m_ambiantBackground;
    sf::Sound m_cardShuffle;
  };

}

#endif // PEM_GAME_SCENE_H
