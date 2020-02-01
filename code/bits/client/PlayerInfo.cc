#include "PlayerInfo.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Text.h>

namespace ggj {
  static constexpr gf::Vector2f CardRelativeSize = { 0.1f, 0.20f };

  PlayerInfo::PlayerInfo(gf::ResourceManager& resources)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_role("unknown") {

  }

  void PlayerInfo::initializeHand(const std::array<CardType, MaxCards>& cards) {
    m_cards = cards;
  }

  bool PlayerInfo::getCardType(const gf::Vector2f& relativePos, CardType& res) const {
    for (unsigned i = 0; i < m_cards.size(); ++i) {
      gf::RectF cardBounds = getCardBounds(i);
      if (cardBounds.contains(relativePos)) {
        res = m_cards[i];
        return true;
      }
    }
    return false;
  }

  gf::RectF PlayerInfo::getCardBounds(std::size_t i) const {
    float xPosition =  0.05f + i * (CardRelativeSize.width + 0.05f);
    float yPosition =  0.75f;

    return gf::RectF::fromPositionSize({xPosition, yPosition}, CardRelativeSize);
  }

  void PlayerInfo::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);

    gf::Text text("Role: " + m_role, m_font);
    text.setCharacterSize(coordinates.getRelativeCharacterSize(0.05f));
    text.setColor(gf::Color::Blue);
    text.setPosition(coordinates.getRelativePoint({0.01f, 0.6f}));

    target.draw(text, states);

    gf::Vector2f CardSize = coordinates.getRelativeSize(CardRelativeSize);
    gf::RectangleShape rect;
    rect.setColor(gf::Color::Yellow);
    rect.setSize(CardSize);

    for (unsigned i = 0; i < m_cards.size(); ++i) {
      auto relativePosition = getCardBounds(i).getTopLeft();
      auto position = coordinates.getRelativePoint(relativePosition);
      rect.setPosition(position);
      target.draw(rect, states);

      // Display card name
      gf::Text text(cardTypeString(m_cards[i]), m_font);
      text.setColor(gf::Color::Red);
      text.setCharacterSize(coordinates.getRelativeCharacterSize(0.015f));
      text.setParagraphWidth(CardSize.width - 0.02f * CardSize.width);
      text.setPosition(position + 0.01f * coordinates.getRelativeSize(CardRelativeSize));
      text.setAlignment(gf::Alignment::Center);
      text.setAnchor(gf::Anchor::TopLeft);

      target.draw(text, states);
    }
  }

}
