#include "MenuEntity.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>

namespace ggj {

  MenuEntity::MenuEntity(gf::ResourceManager& resources, MenuChoice& choice)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_startWidget("Start", m_font)
  , m_creditsWidget("Credits", m_font)
  , m_quitWidget("Quit", m_font)
  {
    m_startWidget.setCallback([&choice]() { choice = MenuChoice::Start; });
    m_container.addWidget(m_startWidget);

    m_creditsWidget.setCallback([&choice]() { choice = MenuChoice::Credits; });
    m_container.addWidget(m_creditsWidget);

    m_quitWidget.setCallback([&choice]() { choice = MenuChoice::Quit; });
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

    auto startPosition = coords.getRelativePoint({ 0.5f, 0.2f });
    m_startWidget.setCharacterSize(characterSize);
    m_startWidget.setAnchor(gf::Anchor::Center);
    m_startWidget.setPosition(startPosition);
    target.draw(m_startWidget, states);

    auto creditsPosition = coords.getRelativePoint({ 0.5f, 0.4f });
    m_creditsWidget.setCharacterSize(characterSize);
    m_creditsWidget.setAnchor(gf::Anchor::Center);
    m_creditsWidget.setPosition(creditsPosition);
    target.draw(m_creditsWidget, states);

    auto quitPosition = coords.getRelativePoint({ 0.5f, 0.6f });
    m_quitWidget.setCharacterSize(characterSize);
    m_quitWidget.setAnchor(gf::Anchor::Center);
    m_quitWidget.setPosition(quitPosition);
    target.draw(m_quitWidget, states);
  }


}


