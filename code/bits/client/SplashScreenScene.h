#ifndef PEM_SPLASH_SCREEN_SCENE_H
#define PEM_SPLASH_SCREEN_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>

#include "BackgroundEntity.h"
#include "MenuEntity.h"
#include "SplashScreenEntity.h"

namespace pem {

  struct Scenes;

  class SplashScreenScene : public gf::Scene {
  public:
    SplashScreenScene(Scenes& scenes, gf::ResourceManager& resources);

  private:
    void doHandleActions(gf::Window& window) override;
    void doUpdate(gf::Time time) override;

  private:
    Scenes& m_scenes;

    gf::Action m_escapeAction;

    SplashScreenEntity m_splashScreenEntity;
  };

}

#endif // PEM_SPLASH_SCREEN_SCENE_H
