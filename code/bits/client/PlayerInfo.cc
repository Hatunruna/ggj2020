#include "PlayerInfo.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

namespace ggj {
  PlayerInfo::PlayerInfo(gf::ResourceManager& resources)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_role("unknown") {

  }

  void PlayerInfo::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);

    gf::Text text("Role: " + m_role, m_font);
    text.setCharacterSize(coordinates.getRelativeCharacterSize(0.05f));
    text.setColor(gf::Color::Blue);
    text.setPosition(coordinates.getRelativePoint({0.01f, 0.6f}));

    target.draw(text, states);
  }

}
