#include "IntroScene.h"

#include <gf/Log.h>

#include "common/Constants.h"

#include "Scenes.h"


namespace pem {

  IntroScene::IntroScene(Scenes& scenes, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_escapeAction("Escape")
  , m_choice(MenuChoice::None)
  , m_menu(resources, m_choice)
  , m_background(resources, false)
  {
    setClearColor(gf::Color::Black);

    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);

    setWorldViewSize(ViewSize);
    setWorldViewCenter(WorldSize * 0.5f);

    addWorldEntity(m_background);
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

  void IntroScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (m_escapeAction.isActive()) {
      m_scenes.popScene();
    }
  }

  void IntroScene::doUpdate([[maybe_unused]] gf::Time time) {
    if (!m_scenes.loadingFinished()) {
      m_choice = MenuChoice::None;
      return;
    }

    switch (m_choice) {
      case MenuChoice::None:
        break;
      case MenuChoice::Start:
        m_scenes.replaceScene(*m_scenes.connection, m_scenes.slideEffect, gf::seconds(0.3f));
        // m_scenes.replaceScene(m_scenes.game, 0.3f, m_scenes.slideEffect);
        break;
      case MenuChoice::Help:
        m_scenes.replaceScene(*m_scenes.help, m_scenes.slideEffect, gf::seconds(0.4f));
        break;
      case MenuChoice::Credits:
        m_scenes.replaceScene(*m_scenes.credits, m_scenes.slideEffect, gf::seconds(0.4f));
        break;
      case MenuChoice::Quit:
        m_scenes.popScene();
        break;
    }

    m_choice = MenuChoice::None;
  }

}
