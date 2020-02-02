#ifndef TMPGAME_SCENES_H
#define TMPGAME_SCENES_H

#include <gf/SceneManager.h>
#include <gf/ResourceManager.h>
#include <gf/Effects.h>

#include "ConnectionScene.h"
#include "CreditsScene.h"
#include "GameScene.h"
#include "HelpScene.h"
#include "IntroScene.h"
#include "LobbyScene.h"
#include "RoomScene.h"

namespace ggj {

  struct Scenes : public gf::SceneManager {
    Scenes(ClientNetwork& network, gf::Path searchDir);
    Scenes(const Scenes&) = delete;
    Scenes(Scenes&&) = delete;
    ~Scenes();

    Scenes& operator=(const Scenes&) = delete;
    Scenes& operator=(Scenes&&) = delete;

    gf::ResourceManager resources;

    IntroScene intro;
    ConnectionScene connection;
    LobbyScene lobby;
    RoomScene room;
    GameScene game;
    CreditsScene credits;
    HelpScene help;

    gf::SlideTransitionEffect slideEffect;
    gf::FadeTransitionEffect fadeEffect;

    gf::Id myPlayerId;
  };

}

#endif // TMPGAME_SCENES_H
