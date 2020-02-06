#include "ShipEntity.h"

#include <gf/AnimatedSprite.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Polygon.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

#include "common/PemTypes.h"

namespace {

}

namespace pem {
  ShipEntity::ShipEntity(gf::ResourceManager& resources, GameModel &model)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_shipTexture(resources.getTexture("image/ship.png"))
  , m_engineTexture(resources.getTexture("image/engine.png"))
  , m_model(model) {
    // Load sprite for animation
    static constexpr gf::Vector2f EngineTextureSize = { 1072.0f / 6432.0f, 1521.0f / 6084.0f };
    for (int j = 0; j < 4; ++j) {
      for (int i = 0; i < 6; ++i) {
        auto rectTexture = gf::RectF::fromPositionSize({ i * EngineTextureSize.width, j * EngineTextureSize.height }, EngineTextureSize);
        m_engineAnimation.addFrame(m_engineTexture, rectTexture, gf::seconds(1.0f/25.0f));
      }
    }
  }

  void ShipEntity::updateMouseCoords(const gf::Vector2f& coords) {
    m_mouseCoords = coords;
  }

  void ShipEntity::updateSelectedPlace() {
    for (auto &location: m_model.placeLocations) {
      if (location.second.placeBounds.contains(m_mouseCoords)) {
        gf::Log::debug("(SHIP) select place: %s\n", placeTypeString(location.first).c_str());
        m_model.selectedPlace = location.first;
        return;
      }
    }
  }

  void ShipEntity::update(gf::Time time) {
    m_engineAnimation.update(time);
  }

  void ShipEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::AnimatedSprite animation;
    animation.setAnimation(m_engineAnimation);
    animation.setPosition({ 625.0f, 0.0f });
    animation.setAnchor(gf::Anchor::TopRight);
    target.draw(animation, states);

    gf::Coordinates coordinates(target);
    gf::Sprite ship(m_shipTexture);
    ship.setPosition({ 0.0f, 0.0f });
    target.draw(ship, states);

    auto drawCursor = [&target, &states, &coordinates](PlaceLocation location, gf::Color4f fillColor) {
      auto characterSize = coordinates.getRelativeCharacterSize(0.1f);

      gf::CircleShape center(30.0f);
      center.setPosition({ location.placeBounds.getCenter() });
      center.setColor(fillColor);
      center.setOutlineColor(gf::Color::Black);
      center.setOutlineThickness(characterSize * 0.05f);
      center.setAnchor(gf::Anchor::Center);
      target.draw(center, states);
    };

    // Display current location on Action and Resolution
    if (m_model.gamePhase == GamePhase::Action || m_model.gamePhase == GamePhase::Resolution) {
      for (const auto &entry: m_model.placeLocations) {
        const auto& key = entry.first;
        const auto& location = entry.second;

        if (m_model.selectedPlace == key) {
          drawCursor(location, gf::Color::White);
        }
      }
    }

    if (m_model.gamePhase == GamePhase::Action) {
      // Draw cursor on action phase
      for (const auto &entry: m_model.placeLocations) {
        const auto& location = entry.second;

        if (!location.placeBounds.contains(m_mouseCoords) || m_model.players.at(m_model.myPlayerId).jail) {
          continue;
        }

        // // Display Hitbox - debug
        // gf::ConvexShape polygonShape(location.placeBounds);
        // polygonShape.setOutlineColor(gf::Color::Violet);
        // polygonShape.setOutlineThickness(5.0f);
        // polygonShape.setColor(gf::Color::Transparent);
        // target.draw(polygonShape, states);

        // Display place name
        gf::Text text(location.name, m_font);
        auto characterSize = coordinates.getRelativeCharacterSize(0.1f);
        text.setCharacterSize(characterSize);
        text.setColor(gf::Color::White);
        text.setOutlineColor(gf::Color::Black);
        text.setOutlineThickness(characterSize * 0.05f);
        text.setPosition(location.titlePosition);
        text.setAnchor(gf::Anchor::BottomLeft);
        target.draw(text, states);

        drawCursor(location, gf::Color::White);
      }
    }
    else if (m_model.gamePhase == GamePhase::Meeting) {
      for (const auto &entry: m_model.placeLocations) {
        const auto& location = entry.second;

        if (location.working) {
          drawCursor(location, gf::Color::Green);
        }
        else {
          drawCursor(location, gf::Color::Red);
        }
      }
    }
  }
}
