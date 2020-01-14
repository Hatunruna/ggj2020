#include "MenuEntity.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>

namespace ggj {

  MenuEntity::MenuEntity(gf::ResourceManager& resources)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_startWidget("Start", m_font)
  , m_quitWidget("Quit", m_font)
  {
    m_startWidget.setCallback([]() { gf::Log::debug("Start!\n"); });
    m_container.addWidget(m_startWidget);

    m_quitWidget.setCallback([]() { gf::Log::debug("Quit!\n"); });
    m_container.addWidget(m_quitWidget);

  }

  void MenuEntity::pointTo(gf::Vector2f coords) {
    m_container.pointTo(coords);
  }

  void MenuEntity::triggerAction() {
    m_container.triggerAction();
  }

  void MenuEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coords(target);
    unsigned characterSize = coords.getRelativeCharacterSize(0.1f);

    auto startPosition = coords.getRelativePoint({ 0.5f, 0.4f });
    m_startWidget.setCharacterSize(characterSize);
    m_startWidget.setAnchor(gf::Anchor::Center);
    m_startWidget.setPosition(startPosition);
    target.draw(m_startWidget, states);

    auto quitPosition = coords.getRelativePoint({ 0.5f, 0.6f });
    m_quitWidget.setCharacterSize(characterSize);
    m_quitWidget.setAnchor(gf::Anchor::Center);
    m_quitWidget.setPosition(quitPosition);
    target.draw(m_quitWidget, states);
  }


}


