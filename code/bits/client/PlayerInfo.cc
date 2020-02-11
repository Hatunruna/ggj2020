#include "PlayerInfo.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/StringUtils.h>
#include <gf/Text.h>

namespace pem {
  static constexpr gf::Vector2i CardTextureSize = { 400, 500 };
  static constexpr float TextureRatio = static_cast<float>(CardTextureSize.x) / static_cast<float>(CardTextureSize.y);
  static constexpr float RelativeCardHeight = 0.30f;

  PlayerInfo::PlayerInfo(gf::ResourceManager& resources, GameModel &model)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_atlas("atlas.xml", resources)
  , m_model(model) {

  }

  void PlayerInfo::updateSelectedCard(const gf::Vector2f &mouseCoords, const gf::Vector2f& screenSize) {
    if (m_model.selectedCard != -1) {
      return;
    }

    for (unsigned i = 0; i < m_model.cards.size(); ++i) {
      gf::RectF cardBounds = getCardBounds(screenSize, i);

      if (cardBounds.contains(mouseCoords)) {
        gf::Log::debug("(PlayerInfo) select card: %s\n", cardTypeString(m_model.cards[i]).c_str());
        m_model.selectedCard = i;
        return;
      }
    }
  }

  void PlayerInfo::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);

    // Rebels win message
    if (m_model.turn <= 0) {
      unsigned characterSize = coordinates.getRelativeCharacterSize(0.075f);
      gf::Text text("The rebels won", m_font, characterSize);
      text.setAnchor(gf::Anchor::Center);
      text.setPosition(coordinates.getCenter());
      text.setColor(gf::Color::White);
      text.setOutlineColor(gf::Color::Black);
      text.setOutlineThickness(characterSize * 0.05f);
      target.draw(text, states);
      return;
    }

    // Protector win message
    if (m_model.distance <= 0.0f) {
      unsigned characterSize = coordinates.getRelativeCharacterSize(0.075f);
      gf::Text text("The protectors won", m_font, characterSize);
      text.setAnchor(gf::Anchor::Center);
      text.setPosition(coordinates.getCenter());
      text.setColor(gf::Color::White);
      text.setOutlineColor(gf::Color::Black);
      text.setOutlineThickness(characterSize * 0.05f);
      target.draw(text, states);
      return;
    }

    // Display turn
    unsigned characterSize = coordinates.getRelativeCharacterSize(0.05f);
    std::string info = "Turn left: " + std::to_string(m_model.turn);
    gf::Text text(info, m_font, characterSize);
    text.setAnchor(gf::Anchor::TopLeft);
    text.setPosition(coordinates.getRelativeSize({0.025f, 0.025f}));
    text.setColor(gf::Color::White);
    text.setOutlineColor(gf::Color::Black);
    text.setOutlineThickness(characterSize * 0.05f);
    target.draw(text, states);

    // Display objectif
    info = "Distance: " + gf::niceNum(m_model.distance, 0.1f);
    text.setString(info);
    text.setAnchor(gf::Anchor::TopRight);
    text.setPosition(coordinates.getRelativeSize({0.975f, 0.025f}));
    text.setColor(gf::Color::White);
    text.setOutlineColor(gf::Color::Black);
    text.setOutlineThickness(characterSize * 0.05f);
    target.draw(text, states);

    // Display role
    constexpr float outlineThickness = 5.0f;
    gf::RectangleShape m_rectRole;
    m_rectRole.setColor(gf::Color::Transparent);
    m_rectRole.setPosition({outlineThickness,outlineThickness});
    auto screenSize = coordinates.getRelativeSize({1.0f, 1.0f});
    m_rectRole.setSize(screenSize - gf::Vector2f(outlineThickness * 2, outlineThickness * 2));
    if (m_model.role == CrewType::Protector) {
      m_rectRole.setOutlineColor(gf::Color::Blue);
    }
    else {
      m_rectRole.setOutlineColor(gf::Color::Red);
    }
    m_rectRole.setOutlineThickness(outlineThickness);
    target.draw(m_rectRole, states);

    // If it's meeting, hide cards
    if (m_model.gamePhase == GamePhase::Meeting) {
      return;
    }

    // Display cards
    for (unsigned i = 0; i < m_model.cards.size(); ++i) {
      auto bounds = getCardBounds(coordinates.getRelativeSize({ 1.0f, 1.0f }), i);
      auto cardSize = bounds.getSize();

      float offsetSelected = 0.0f;
      if (m_model.selectedCard == static_cast<int>(i)) {
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
      card.setTextureRect(m_atlas.getTextureRect(cardTypeString(m_model.cards[i])));
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
      gf::Text text(cardTypeString(m_model.cards[i]), m_font);
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

  gf::RectF PlayerInfo::getCardBounds(gf::Vector2f screeSize, std::size_t i) const {
    float newHeight = screeSize.height * RelativeCardHeight;
    float newWidth = newHeight * TextureRatio;
    float xPosition = 0.02f * screeSize.width + i * (newWidth + 0.01f * screeSize.width);
    float yPositon = 0.98f * screeSize.height - newHeight;

    return gf::RectF::fromPositionSize({xPosition, yPositon}, {newWidth, newHeight});
  }

}
