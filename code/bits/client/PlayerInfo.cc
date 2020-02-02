#include "PlayerInfo.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace ggj {
  static constexpr gf::Vector2i CardTextureSize = { 400, 500 };
  static constexpr float TextureRatio = static_cast<float>(CardTextureSize.x) / static_cast<float>(CardTextureSize.y);
  static constexpr float RelativeCardHeight = 0.30f;

  PlayerInfo::PlayerInfo(gf::ResourceManager& resources)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_emptyCardTexture(resources.getTexture("image/empty_card.png"))
  , m_atlas("atlas.xml", resources)
  , m_role("unknown") {

  }

  void PlayerInfo::initializeHand(const std::array<CardType, MaxCards>& cards) {
    m_cards = cards;
  }

  bool PlayerInfo::getCardType(const gf::Vector2f& screenPosition, const gf::Vector2f& screenSize, CardType& res) const {
    for (unsigned i = 0; i < m_cards.size(); ++i) {
      gf::RectF cardBounds = getCardBounds(screenSize, i);
      if (cardBounds.contains(screenPosition)) {
        res = m_cards[i];
        return true;
      }
    }
    return false;
  }

  gf::RectF PlayerInfo::getCardBounds(gf::Vector2f screeSize, std::size_t i) const {
    float newHeight = screeSize.height * RelativeCardHeight;
    float newWidth = newHeight * TextureRatio;
    float xPosition = 0.01f * screeSize.width + i * (newWidth + 0.01f * screeSize.width);
    float yPositon = 0.99f * screeSize.height - newHeight;

    return gf::RectF::fromPositionSize({xPosition, yPositon}, {newWidth, newHeight});
  }

  void PlayerInfo::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);

    gf::Text text("Role: " + m_role, m_font);
    text.setCharacterSize(coordinates.getRelativeCharacterSize(0.05f));
    text.setColor(gf::Color::Blue);
    text.setPosition(coordinates.getRelativePoint({0.01f, 0.6f}));
    target.draw(text, states);



    for (unsigned i = 0; i < m_cards.size(); ++i) {
      auto bounds = getCardBounds(coordinates.getRelativeSize({ 1.0f, 1.0f }), i);
      auto cardSize = bounds.getSize();

      // // Hitbox debug
      // gf::RectangleShape rect;
      // rect.setColor(gf::Color::Yellow);
      // rect.setSize(cardSize);
      // rect.setPosition(bounds.getBottomLeft());
      // rect.setAnchor(gf::Anchor::BottomLeft);
      // target.draw(rect, states);

      // Empty card
      gf::Sprite card(m_atlas.getTexture());
      card.setTextureRect(m_atlas.getTextureRect(cardTypeString(m_cards[i])));
      gf::Vector2f scaleFactor = cardSize / CardTextureSize;
      card.setScale(scaleFactor);
      card.setPosition(bounds.getBottomLeft());
      card.setAnchor(gf::Anchor::BottomLeft);
      target.draw(card, states);

      // Get Title position
      float titleMargin = 20.0f * scaleFactor.width;
      gf::Vector2f textBoxSize = gf::Vector2f(360.0f, 100.0f) * scaleFactor;
      gf::Vector2f textBoxPosition = bounds.getBottomLeft();
      textBoxPosition.x += titleMargin;
      textBoxPosition.y -= titleMargin;

      // // Text box debug
      // gf::RectangleShape rect;
      // rect.setColor(gf::Color::Yellow);
      // rect.setSize(textBoxSize);
      // rect.setPosition(textBoxPosition);
      // rect.setAnchor(gf::Anchor::BottomLeft);
      // target.draw(rect, states);

      // Display card name
      gf::Text text(cardTypeString(m_cards[i]), m_font);
      text.setColor(gf::Color::Black);
      text.setCharacterSize(coordinates.getRelativeCharacterSize(0.020f));
      text.setParagraphWidth(textBoxSize.width);
      // NOTE: hacks... @ahugeat
      textBoxPosition.height -= textBoxSize.height * 0.5f;
      text.setPosition(textBoxPosition);
      text.setAlignment(gf::Alignment::Center);
      text.setAnchor(gf::Anchor::CenterLeft);
      target.draw(text, states);
    }
  }

}
