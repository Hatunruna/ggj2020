#include "ShipEntity.h"

#include <gf/RenderTarget.h>

namespace ggj {
  ShipEntity::ShipEntity() {
    m_rect.setColor(gf::Color::White);
    m_rect.setSize({6000.0f, 500.0f});
    m_rect.setPosition({1000.f, 500.0f});
    m_rect.setAnchor(gf::Anchor::Center);
  }

  void ShipEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    target.draw(m_rect, states);
  }
}
