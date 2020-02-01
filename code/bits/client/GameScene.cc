#include "GameScene.h"

#include <gf/Shapes.h>

#include "common/Constants.h"

#include "Scenes.h"

namespace ggj {

  GameScene::GameScene(Scenes& scenes, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_adaptator(m_scenes.getRenderer(), getWorldView())
  , m_escapeAction("Escape")
  {
    setWorldViewSize({2000.0f, 1000.0f});
    setWorldViewCenter({ 1000.0f, 500.0f});

    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);

    addWorldEntity(m_ship);

    getWorldView().setViewport(gf::RectF::fromPositionSize({0.0f, 0.0f}, {1.0f, 2.f / 3.f}));
  }

  void GameScene::doHandleActions(gf::Window& window) {
    if (m_escapeAction.isActive()) {
      m_scenes.setClearColor(gf::Color::White);
      m_scenes.replaceScene(m_scenes.intro);
    }
  }

  void GameScene::doProcessEvent(gf::Event &event) {
    m_adaptator.processEvent(event);
  }

  void GameScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
    // Default display
    renderWorldEntities(target, states);
    renderHudEntities(target, states);
  }

}
