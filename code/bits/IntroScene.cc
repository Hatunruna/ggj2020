#include "IntroScene.h"

#include "Constants.h"
#include "Scenes.h"

namespace ggj {

  IntroScene::IntroScene(Scenes& scenes, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_escapeAction("Escape")
  , m_menu(resources)
  {
    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);

    addHudEntity(m_menu);
  }

  void IntroScene::doProcessEvent(gf::Event& event) {
    switch (event.type) {
      case gf::EventType::MouseMoved:
        m_menu.pointTo(m_scenes.getRenderer().mapPixelToCoords(event.mouseCursor.coords));
        break;

      case gf::EventType::MouseButtonPressed:
        m_menu.pointTo(m_scenes.getRenderer().mapPixelToCoords(event.mouseButton.coords));
        m_menu.triggerAction();
        break;

      default:
        break;
    }
  }

  void IntroScene::doHandleActions(gf::Window& window) {
    if (m_escapeAction.isActive()) {
      m_scenes.popScene();
    }
  }

}
