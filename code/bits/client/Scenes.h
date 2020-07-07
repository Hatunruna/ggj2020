#ifndef PEM_SCENES_H
#define PEM_SCENES_H

#include <atomic>

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
#include "SplashScreenScene.h"

namespace pem {

  struct Scenes : public gf::SceneManager {
    Scenes(gf::Path searchDir);
    Scenes(const Scenes&) = delete;
    Scenes(Scenes&&) = delete;
    ~Scenes();

    Scenes& operator=(const Scenes&) = delete;
    Scenes& operator=(Scenes&&) = delete;

    void loadingAllAssets(ClientNetwork& network);
    void loadingSplashScreen();
    void loadingMainAssets(ClientNetwork& network);
    bool loadingFinished();

    gf::ResourceManager resources;

    std::unique_ptr<SplashScreenScene> splashScreen;
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
    gf::BlackoutSegueEffect blackoutEffect;

    gf::Id myPlayerId;

    std::atomic_bool m_loadingFinished;
  };

}

#endif // PEM_SCENES_H
