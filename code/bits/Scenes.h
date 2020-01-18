#ifndef TMPGAME_SCENES_H
#define TMPGAME_SCENES_H

#include <gf/SceneManager.h>
#include <gf/ResourceManager.h>

#include "ConnectionScene.h"
#include "IntroScene.h"
#include "LobbyScene.h"
#include "RoomScene.h"
#include "WaitingScene.h"

namespace ggj {

  struct Scenes : gf::SceneManager {
    Scenes(ClientNetwork& network, gf::Path searchDir);

    gf::ResourceManager resources;

    IntroScene intro;
    ConnectionScene connection;
    LobbyScene lobby;
    RoomScene room;
    WaitingScene waiting;
  };

}

#endif // TMPGAME_SCENES_H
