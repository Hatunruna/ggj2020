#include "WaitingScene.h"

#include "../common/Constants.h"

#include "Scenes.h"

namespace ggj {

  WaitingScene::WaitingScene(Scenes& scenes, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_escapeAction("Escape")
  {
    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);
  }

  void WaitingScene::doHandleActions(gf::Window& window) {
    if (m_escapeAction.isActive()) {
      m_scenes.replaceScene(m_scenes.intro);
    }
  }

}
