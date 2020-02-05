#include "CreditsScene.h"

#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/Unused.h>

#include "common/Constants.h"

#include "Scenes.h"

namespace pem {

  CreditsScene::CreditsScene(Scenes& scenes, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_escapeAction("Escape")
  , m_font(resources.getFont("DejaVuSans.ttf"))
  , m_backgorund(resources)
  {
    setClearColor(gf::Color::Black);

    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);

    addHudEntity(m_backgorund);
  }

  void CreditsScene::doHandleActions(gf::Window& window) {
    gf::unused(window);

    if (m_escapeAction.isActive()) {
      m_scenes.replaceScene(m_scenes.intro, m_scenes.fadeEffect, gf::seconds(0.4f));
    }
  }

  void CreditsScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
    // Default display
    renderWorldEntities(target, states);
    renderHudEntities(target, states);

    gf::Coordinates coords(target);
    unsigned characterSize = coords.getRelativeCharacterSize(0.1f);

    auto titlePosition = coords.getRelativePoint({ 0.5f, 0.1f });
    gf::Text title("Credits", m_font);
    title.setCharacterSize(characterSize);
    title.setAnchor(gf::Anchor::Center);
    title.setPosition(titlePosition);
    title.setColor(gf::Color::White);
    title.setOutlineColor(gf::Color::Black);
    title.setOutlineThickness(characterSize * 0.05f);
    target.draw(title, states);
  }

}
