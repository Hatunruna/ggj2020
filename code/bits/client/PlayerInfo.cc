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
  , m_selectedCard(-1) {

  }

  void PlayerInfo::initializeHand(const std::array<CardType, MaxCards>& cards) {
    m_cards = cards;
  }

  bool PlayerInfo::getCardType(const gf::Vector2f& screenPosition, const gf::Vector2f& screenSize, CardType& res) {
    for (unsigned i = 0; i < m_cards.size(); ++i) {
      gf::RectF cardBounds = getCardBounds(screenSize, i);
      if (cardBounds.contains(screenPosition)) {
        res = m_cards[i];
        m_selectedCard = i;
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

    constexpr float outlineThickness = 5.f;
    gf::RectangleShape m_rectRole;
    m_rectRole.setColor(gf::Color::Transparent);
    m_rectRole.setPosition({outlineThickness,outlineThickness});
    auto screenSize = coordinates.getRelativeSize({1.0f, 2.f / 3.f});
    m_rectRole.setSize(screenSize - gf::Vector2f(outlineThickness * 2, outlineThickness));
    if (m_role == CrewType::Protector)
    {
      m_rectRole.setOutlineColor(gf::Color::Blue);
    }
    else
    {
      m_rectRole.setOutlineColor(gf::Color::Red);
    }
    m_rectRole.setOutlineThickness(outlineThickness);
    target.draw(m_rectRole, states);

    for (unsigned i = 0; i < m_cards.size(); ++i) {
      auto bounds = getCardBounds(coordinates.getRelativeSize({ 1.0f, 1.0f }), i);
      auto cardSize = bounds.getSize();

      float offsetSelected = 0.0f;
      if (m_selectedCard == static_cast<int>(i)) {
        offsetSelected = coordinates.getRelativeSize({ 0.0f, 0.05f }).height;
      }

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
      auto posCard = bounds.getBottomLeft();
      posCard.y -= offsetSelected;
      card.setPosition(posCard);
      card.setAnchor(gf::Anchor::BottomLeft);
      target.draw(card, states);

      // Get Title position
      float titleMargin = 20.0f * scaleFactor.width;
      gf::Vector2f textBoxSize = gf::Vector2f(360.0f, 100.0f) * scaleFactor;
      gf::Vector2f textBoxPosition = bounds.getBottomLeft();
      textBoxPosition.x += titleMargin;
      textBoxPosition.y -= titleMargin + offsetSelected;

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
