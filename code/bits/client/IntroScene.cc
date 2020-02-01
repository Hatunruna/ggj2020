#include "IntroScene.h"

#include "common/Constants.h"

#include "Scenes.h"

#include <gf/Log.h>

namespace ggj {

  IntroScene::IntroScene(Scenes& scenes, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_escapeAction("Escape")
  , m_choice(MenuChoice::None)
  , m_menu(resources, m_choice)
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

  void IntroScene::doUpdate(gf::Time time) {
    switch (m_choice) {
      case MenuChoice::None:
        break;
      case MenuChoice::Start:
        // m_scenes.replaceScene(m_scenes.connection);
        m_scenes.replaceScene(m_scenes.game);
        m_scenes.setClearColor(gf::Color::Black);
        break;
      case MenuChoice::Help:
        m_scenes.replaceScene(m_scenes.help);
        break;
      case MenuChoice::Credits:
        m_scenes.replaceScene(m_scenes.credits);
        break;
      case MenuChoice::Quit:
        m_scenes.popScene();
        break;
    }

    m_choice = MenuChoice::None;
  }

}
