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
    case pem::PlaceType::Armory:
      return { 2277.0f, 258.0f };

    case pem::PlaceType::Bathroom:
      return { 2070.0f, 342.0f };

    case pem::PlaceType::CommunicationCenter:
      return { 2985.0f, 438.0f };

    case pem::PlaceType::Dormitory:
      return { 1599.0f, 288.0f };

    case pem::PlaceType::GreenHouse:
      return { 2028.0f, 228.0f };

    case pem::PlaceType::Infirmery:
      return { 1629.0f, 771.0f };

    case pem::PlaceType::LeftEngine:
      return { 673.0f, 219.0f };

    case pem::PlaceType::LifeSupport:
      return { 2619.0f, 831.0f };

    case pem::PlaceType::MainBridge:
      return { 3315.0f, 654.0f };

    case pem::PlaceType::MidEngine:
      return { 3504.0f, 658.0f };

    case pem::PlaceType::Navigation:
      return { 2898.0f, 642.0f };

    case pem::PlaceType::Prison:
      return { 479.0f, 414.0f };

    case pem::PlaceType::Refectory:
      return { 1233.0f, 582.0f };

    case pem::PlaceType::RightEngine:
      return { 153.0f, 633.0f };

    case pem::PlaceType::Storage:
      return { 1728.0f, 717.0f };

    default:
      assert(false);
      break;
    }

    return { 0.0f, 0.0f };
  }
}

namespace pem {
  PlaceEntity::PlaceEntity(gf::Texture &workingTexture,
    std::vector<gf::Ref<gf::Texture>> brokenTextures,
    GameModel &model, PlaceType place,
    int lastNumberFrame, gf::Vector2i tilesetLayout)
  : m_model(model)
  , m_place(place)
  , m_workingTexture(workingTexture)
  , m_position(getPlaceLocation(place)) {

    // Add tileset without empty frame
    for (int i = 0; i < static_cast<int>(brokenTextures.size()) - 1; ++i) {
      auto &texture = brokenTextures[i];

      m_brokenAnimation.addTileset(texture, tilesetLayout, gf::seconds(1.0f / 25.0f), tilesetLayout.width * tilesetLayout.height);
    }

    // Add the last tileset with probably missing frames
    m_brokenAnimation.addTileset(brokenTextures.back(), tilesetLayout, gf::seconds(1.0f / 25.0f), lastNumberFrame);
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
    else {
      gf::Sprite sprite;
      sprite.setTexture(m_workingTexture);
      sprite.setPosition(m_position);
      target.draw(sprite);
    }
  }

  ShipEntity::ShipEntity(gf::ResourceManager& resources, GameModel &model)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_shipTexture(resources.getTexture("images/ship.png"))
  , m_model(model) {
    // Load sprites for animations
    m_leftFlameWorkingAnimation.addTileset(resources.getTexture("animations/left_flame_working.png"), { 6, 4}, gf::seconds(1.0f / 25.0f), 24);
    m_leftFlameBrokenAnimation.addTileset(resources.getTexture("animations/left_flame_broken.png"), { 6, 4}, gf::seconds(1.0f / 25.0f), 24);
    m_rightFlameWorkingAnimation.addTileset(resources.getTexture("animations/right_flame_working.png"), { 6, 4}, gf::seconds(1.0f / 25.0f), 24);
    m_rightFlameBrokenAnimation.addTileset(resources.getTexture("animations/right_flame_broken.png"), { 6, 4}, gf::seconds(1.0f / 25.0f), 24);

    // Add all animations for broken place
    m_places.emplace_back(
      resources.getTexture("images/left_engine.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/left_engine_broken.png")
      }),
      m_model, PlaceType::LeftEngine
    );
    m_places.emplace_back(
      resources.getTexture("images/right_engine.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/right_engine_broken.png")
      }),
      m_model, PlaceType::RightEngine
    );
    m_places.emplace_back(
      resources.getTexture("images/prison.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/prison_broken.png")
      }),
      m_model, PlaceType::Prison
    );
    m_places.emplace_back(
      resources.getTexture("images/main_bridge.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/main_bridge_broken_0.png"),
        resources.getTexture("animations/main_bridge_broken_1.png"),
        resources.getTexture("animations/main_bridge_broken_2.png"),
        resources.getTexture("animations/main_bridge_broken_3.png")
      }),
      m_model, PlaceType::MainBridge, 20
    );
    m_places.emplace_back(
      resources.getTexture("images/hyperdrive.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/hyperdrive_broken_0.png"),
        resources.getTexture("animations/hyperdrive_broken_1.png"),
        resources.getTexture("animations/hyperdrive_broken_2.png"),
        resources.getTexture("animations/hyperdrive_broken_3.png"),
        resources.getTexture("animations/hyperdrive_broken_4.png")
      }),
      m_model, PlaceType::MidEngine, 12
    );
    m_places.emplace_back(
      resources.getTexture("images/navigation.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/navigation_broken_0.png"),
        resources.getTexture("animations/navigation_broken_1.png"),
        resources.getTexture("animations/navigation_broken_2.png"),
        resources.getTexture("animations/navigation_broken_3.png"),
        resources.getTexture("animations/navigation_broken_4.png")
      }),
      m_model, PlaceType::Navigation, 15
    );
    m_places.emplace_back(
      resources.getTexture("images/communication_center.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/communication_center_broken_0.png"),
        resources.getTexture("animations/communication_center_broken_1.png"),
        resources.getTexture("animations/communication_center_broken_2.png"),
        resources.getTexture("animations/communication_center_broken_3.png"),
        resources.getTexture("animations/communication_center_broken_4.png")
      }),
      m_model, PlaceType::CommunicationCenter, 6
    );
    m_places.emplace_back(
      resources.getTexture("images/life_support.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/life_support_broken_0.png"),
        resources.getTexture("animations/life_support_broken_1.png"),
        resources.getTexture("animations/life_support_broken_2.png"),
        resources.getTexture("animations/life_support_broken_3.png"),
        resources.getTexture("animations/life_support_broken_4.png")
      }),
      m_model, PlaceType::LifeSupport, 8
    );
    m_places.emplace_back(
      resources.getTexture("images/greenhouse.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/greenhouse_broken_0.png"),
        resources.getTexture("animations/greenhouse_broken_1.png"),
        resources.getTexture("animations/greenhouse_broken_2.png"),
        resources.getTexture("animations/greenhouse_broken_3.png"),
        resources.getTexture("animations/greenhouse_broken_4.png")
      }),
      m_model, PlaceType::GreenHouse, 6, gf::vec(3, 3)
    );
    m_places.emplace_back(
      resources.getTexture("images/armory.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/armory_broken_0.png"),
        resources.getTexture("animations/armory_broken_1.png"),
        resources.getTexture("animations/armory_broken_2.png")
      }), m_model, PlaceType::Armory, 12
    );
    m_places.emplace_back(
      resources.getTexture("images/storage.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/storage_broken_0.png"),
        resources.getTexture("animations/storage_broken_1.png"),
      }),
      m_model, PlaceType::Storage, 12
    );
    m_places.emplace_back(
      resources.getTexture("images/bathroom.png"),
        std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/bathroom_broken_0.png"),
        resources.getTexture("animations/bathroom_broken_1.png"),
      }),
      m_model, PlaceType::Bathroom, 16
    );
    m_places.emplace_back(
      resources.getTexture("images/infirmery.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/infirmery_broken_0.png"),
        resources.getTexture("animations/infirmery_broken_1.png"),
      }),
      m_model, PlaceType::Infirmery, 12
    );
    m_places.emplace_back(
      resources.getTexture("images/dormitory.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/dormitory_broken_0.png"),
        resources.getTexture("animations/dormitory_broken_1.png"),
        resources.getTexture("animations/dormitory_broken_2.png"),
        resources.getTexture("animations/dormitory_broken_3.png"),
      }),
      m_model, PlaceType::Dormitory, 7
    );
    m_places.emplace_back(
      resources.getTexture("images/refectory.png"),
      std::vector<gf::Ref<gf::Texture>>({
        resources.getTexture("animations/refectory_broken_0.png"),
        resources.getTexture("animations/refectory_broken_1.png"),
      }),
      m_model, PlaceType::Refectory, 12
    );
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

    // Display the broken animations
    for (auto &place: m_places) {
      place.render(target, states);
    }

    // Display the ship
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
