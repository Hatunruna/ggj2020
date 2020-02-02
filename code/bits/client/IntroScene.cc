#include "IntroScene.h"

#include <gf/Log.h>
#include <gf/Unused.h>

#include "common/Constants.h"

#include "Scenes.h"


namespace ggj {

  IntroScene::IntroScene(Scenes& scenes, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_escapeAction("Escape")
  , m_choice(MenuChoice::None)
  , m_menu(resources, m_choice)
  , m_backgorund(resources)
  {
    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);

    addHudEntity(m_backgorund);
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
    gf::unused(window);

    if (m_escapeAction.isActive()) {
      m_scenes.popScene();
    }
  }

  void IntroScene::doUpdate(gf::Time time) {
    gf::unused(time);

    switch (m_choice) {
      case MenuChoice::None:
        break;
      case MenuChoice::Start:
        m_scenes.transitionToScene(m_scenes.connection, 0.5f, m_scenes.glitchEffect);
        // m_scenes.replaceScene(m_scenes.game);
        // m_scenes.setClearColor(gf::Color::Gray(0.85f));
        break;
      case MenuChoice::Help:
        m_scenes.transitionToScene(m_scenes.help, 0.4f, m_scenes.slideEffect);
        break;
      case MenuChoice::Credits:
        m_scenes.transitionToScene(m_scenes.credits, 0.4f, m_scenes.slideEffect);
        break;
      case MenuChoice::Quit:
        m_scenes.popScene();
        break;
    }

    m_choice = MenuChoice::None;
  }

}
