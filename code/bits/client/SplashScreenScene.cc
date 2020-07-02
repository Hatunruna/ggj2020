#include "SplashScreenScene.h"

#include <gf/Log.h>
#include <gf/Unused.h>

#include "common/Constants.h"

#include "Scenes.h"

namespace pem {

  SplashScreenScene::SplashScreenScene(Scenes& scenes, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_escapeAction("Escape")
  , m_splashScreenEntity(
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/splash_screen_0.png"),
        resources.getTexture("animations/splash_screen_1.png"),
        resources.getTexture("animations/splash_screen_2.png"),
        resources.getTexture("animations/splash_screen_3.png"),
        resources.getTexture("animations/splash_screen_4.png"),
        resources.getTexture("animations/splash_screen_5.png")
      })) {
    setClearColor(gf::Color::Black);

    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);

    addHudEntity(m_splashScreenEntity);
  }

  void SplashScreenScene::doHandleActions(gf::Window& window) {
    gf::unused(window);

    if (m_escapeAction.isActive()) {
      m_scenes.popScene();
    }
  }

  void SplashScreenScene::doUpdate(gf::Time time) {
    gf::unused(time);

    // if (!m_scenes.loadingFinished()) {
    //   m_choice = MenuChoice::None;
    //   return;
    // }
  }

}
