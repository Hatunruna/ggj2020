#ifndef PEM_SCENES_H
#define PEM_SCENES_H

#include <gf/SceneManager.h>
#include <gf/ResourceManager.h>
#include <gf/SegueEffects.h>

#include "ConnectionScene.h"
#include "CreditsScene.h"
#include "GameScene.h"
#include "HelpScene.h"
#include "IntroScene.h"
#include "LobbyScene.h"
#include "RoomScene.h"

namespace pem {

  struct Scenes : public gf::SceneManager {
    Scenes(gf::Path searchDir);
    Scenes(const Scenes&) = delete;
    Scenes(Scenes&&) = delete;
    ~Scenes();

    Scenes& operator=(const Scenes&) = delete;
    Scenes& operator=(Scenes&&) = delete;

    void loadingAssets(ClientNetwork& network);
    void loadingAsynchronousAssets(ClientNetwork& network);
    bool loadingFinished();

    gf::ResourceManager resources;

    std::unique_ptr<IntroScene> intro;
    std::unique_ptr<ConnectionScene> connection;
    std::unique_ptr<LobbyScene> lobby;
    std::unique_ptr<RoomScene> room;
    std::unique_ptr<GameScene> game;
    std::unique_ptr<CreditsScene> credits;
    std::unique_ptr<HelpScene> help;

    gf::SlideSegueEffect slideEffect;
    gf::FadeSegueEffect fadeEffect;
    gf::GlitchSegueEffect glitchEffect;

    gf::Id myPlayerId;

    bool m_asyncLoading;
    bool m_loadingFinished;
  };

}

#endif // PEM_SCENES_H
