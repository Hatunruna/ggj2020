#include "SplashScreenScene.h"

#include <gf/Unused.h>

#include "common/Constants.h"

#include "Scenes.h"

namespace pem {

  SplashScreenScene::SplashScreenScene(Scenes& scenes)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_escapeAction("Escape")
  , m_splashScreenEntity(std::make_unique<SplashScreenEntity>(
      m_scenes.resources.getTexture("images/splash_screen.png")
    )) {
    setClearColor(gf::Color::Black);

    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);
    addHudEntity(*m_splashScreenEntity);
  }

  void SplashScreenScene::loadAnimation() {
    m_splashScreenEntity->loadAnimation(std::vector<gf::Ref<gf::Texture>>({
      m_scenes.resources.getTexture("animations/splash_screen_0.png"),
      m_scenes.resources.getTexture("animations/splash_screen_1.png"),
      m_scenes.resources.getTexture("animations/splash_screen_2.png"),
      m_scenes.resources.getTexture("animations/splash_screen_3.png"),
      m_scenes.resources.getTexture("animations/splash_screen_4.png"),
      m_scenes.resources.getTexture("animations/splash_screen_5.png")
    }));
  }

  void SplashScreenScene::doHandleActions(gf::Window& window) {
    gf::unused(window);

    if (m_escapeAction.isActive()) {
      m_scenes.popScene();
    }
  }

  void SplashScreenScene::doUpdate(gf::Time time) {
    gf::unused(time);

    if (!isActive()) {
      return;
    }

    if (m_scenes.loadingFinished()) {
      m_scenes.replaceScene(*m_scenes.intro, m_scenes.slideEffect, gf::seconds(0.3f));
      return;
    }
  }

}
