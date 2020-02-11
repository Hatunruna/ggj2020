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
  gf::Vector2f getPlaceLocation(pem::PlaceType place) {
    switch (place) {
    case pem::PlaceType::LeftEngine:
      return { 721.0f, 219.0f };
      break;

    case pem::PlaceType::RightEngine:
      return { 347.0f, 661.0f };
      break;

    default:
      assert(false);
      break;
    }

    return { 0.0f, 0.0f };
  }

  gf::Vector2f getPlaceSize(pem::PlaceType place) {
    switch (place) {
    case pem::PlaceType::LeftEngine:
      return { 357.0f / 1428.0f, 203.0f / 1218.0f };
      break;

    case pem::PlaceType::RightEngine:
      return { 336.0f / 1344.0f, 208.0f / 1248.0f };
      break;

    default:
      assert(false);
      break;
    }

    return { 0.0f, 0.0f };
  }
}

namespace pem {
  PlaceEntity::PlaceEntity(gf::Texture& texture, GameModel &model, PlaceType place)
  : m_model(model)
  , m_place(place)
  , m_position(getPlaceLocation(place)) {
    gf::Vector2f TextureSize = getPlaceSize(place);
    m_brokenAnimation.addTileset(texture, TextureSize, { 4, 6 }, 24, gf::seconds(1.0f / 25.0f));
  }

  void PlaceEntity::update(gf::Time time) {
    if (!m_model.placeLocations.at(m_place).working) {
      m_brokenAnimation.update(time);
    }
    else {
      m_brokenAnimation.reset();
    }
  }

  void PlaceEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (!m_model.placeLocations.at(m_place).working) {
      gf::AnimatedSprite animation;
      animation.setAnimation(m_brokenAnimation);
      animation.setPosition(m_position);
      target.draw(animation, states);
    }
  }

  ShipEntity::ShipEntity(gf::ResourceManager& resources, GameModel &model)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_shipTexture(resources.getTexture("images/ship.png"))
  , m_model(model) {
    // Load sprites for animations
    constexpr gf::Vector2f FlameTextureSize = { 1051.0f / 6306.0f, 1521.0f / 6084.0f };
    m_leftFlameWorkingAnimation.addTileset(resources.getTexture("animations/left_flame_working.png"), FlameTextureSize, { 6, 4}, 24, gf::seconds(1.0f / 25.0f));
    m_leftFlameBrokenAnimation.addTileset(resources.getTexture("animations/left_flame_broken.png"), FlameTextureSize, { 6, 4}, 24, gf::seconds(1.0f / 25.0f));
    m_rightFlameWorkingAnimation.addTileset(resources.getTexture("animations/right_flame_working.png"), FlameTextureSize, { 6, 4}, 24, gf::seconds(1.0f / 25.0f));
    m_rightFlameBrokenAnimation.addTileset(resources.getTexture("animations/right_flame_broken.png"), FlameTextureSize, { 6, 4}, 24, gf::seconds(1.0f / 25.0f));

    // Add all animations for broken place
    m_places.emplace_back(resources.getTexture("animations/left_engine_broken.png"), m_model, PlaceType::LeftEngine);
    m_places.emplace_back(resources.getTexture("animations/right_engine_broken.png"), m_model, PlaceType::RightEngine);
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
    if (m_model.placeLocations.at(PlaceType::LeftEngine).working) {
      m_leftFlameWorkingAnimation.update(time);
      m_leftFlameBrokenAnimation.reset();
    }
    else {
      m_leftFlameBrokenAnimation.update(time);
      m_leftFlameWorkingAnimation.reset();
    }

    if (m_model.placeLocations.at(PlaceType::RightEngine).working) {
      m_rightFlameWorkingAnimation.update(time);
      m_rightFlameBrokenAnimation.reset();
    }
    else {
      m_rightFlameBrokenAnimation.update(time);
      m_rightFlameWorkingAnimation.reset();
    }

    for (auto &place: m_places) {
      place.update(time);
    }
  }

  void ShipEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    // Display the right animation for left flame
    if (m_model.placeLocations.at(PlaceType::LeftEngine).working) {
      gf::AnimatedSprite animation;
      animation.setAnimation(m_leftFlameWorkingAnimation);
      animation.setPosition({ 625.0f, 0.0f });
      animation.setAnchor(gf::Anchor::TopRight);
      target.draw(animation, states);
    }
    else {
      gf::AnimatedSprite animation;
      animation.setAnimation(m_leftFlameBrokenAnimation);
      animation.setPosition({ 625.0f, 0.0f });
      animation.setAnchor(gf::Anchor::TopRight);
      target.draw(animation, states);
    }

    // Display the right animation for right flame
    if (m_model.placeLocations.at(PlaceType::RightEngine).working) {
      gf::AnimatedSprite animation;
      animation.setAnimation(m_rightFlameWorkingAnimation);
      animation.setPosition({ 625.0f, 0.0f });
      animation.setAnchor(gf::Anchor::TopRight);
      target.draw(animation, states);
    }
    else {
      gf::AnimatedSprite animation;
      animation.setAnimation(m_rightFlameBrokenAnimation);
      animation.setPosition({ 625.0f, 0.0f });
      animation.setAnchor(gf::Anchor::TopRight);
      target.draw(animation, states);
    }

    // Display the ship
    gf::Coordinates coordinates(target);
    gf::Sprite ship(m_shipTexture);
    ship.setPosition({ 0.0f, 0.0f });
    target.draw(ship, states);

    // Display the broken animations
    for (auto &place: m_places) {
      place.render(target, states);
    }

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
