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
  struct PlaceLocation {
    gf::Polygon placeBounds;
    gf::Vector2f titlePosition;
    std::string name;
    bool state = true;
  };

  std::map<ggj::PlaceType, PlaceLocation> placeLocations;

  void initializeBounds() {
    {
      gf::Polygon polygon;
      polygon.addPoint({ 2311.21f, 609.72f });
      polygon.addPoint({ 2490.06f, 417.526f });
      polygon.addPoint({ 2653.45f, 444.035f });
      polygon.addPoint({ 2410.57f, 715.758f });
      polygon.addPoint({ 2337.7f, 704.713f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 2450.0f, 350.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::Armory);

      placeLocations.emplace(ggj::PlaceType::Armory, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 2256.01f, 444.035f });
      polygon.addPoint({ 2410.57f, 468.336f });
      polygon.addPoint({ 2291.33f, 598.674f });
      polygon.addPoint({ 2130.15f, 576.583f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 2200.0f, 370.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::Bathroom);

      placeLocations.emplace(ggj::PlaceType::Bathroom, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 3017.78f, 766.568f });
      polygon.addPoint({ 3209.88f, 558.91f });
      polygon.addPoint({ 3450.56f, 583.211f });
      polygon.addPoint({ 3340.16f, 731.222f });
      polygon.addPoint({ 3150.27f, 777.614f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 3165.0f, 490.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::CommunicationCenter);

      placeLocations.emplace(ggj::PlaceType::CommunicationCenter, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 1633.34f, 660.53f });
      polygon.addPoint({ 1816.6f, 684.831f });
      polygon.addPoint({ 2105.86f, 587.629f });
      polygon.addPoint({ 2271.46f, 386.598f });
      polygon.addPoint({ 1927.01f, 329.161f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 1870.0f, 275.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::Dormitory);

      placeLocations.emplace(ggj::PlaceType::Dormitory, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 2635.79f, 497.055f });
      polygon.addPoint({ 2872.05f, 417.526f });
      polygon.addPoint({ 3119.35f, 450.663f });
      polygon.addPoint({ 3185.59f, 552.283f });
      polygon.addPoint({ 2629.17f, 1153.17f });
      polygon.addPoint({ 2403.95f, 1228.28f });
      polygon.addPoint({ 2167.68f, 1203.98f });
      polygon.addPoint({ 2070.53f, 1093.52f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 2660.0f, 350.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::GreenHouse);

      placeLocations.emplace(ggj::PlaceType::GreenHouse, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 1701.79f, 1036.08f });
      polygon.addPoint({ 1876.22f, 848.306f });
      polygon.addPoint({ 2046.24f, 868.188f });
      polygon.addPoint({ 1871.81f, 1049.34f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 1820.0f, 770.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::Infirmery);

      placeLocations.emplace(ggj::PlaceType::Infirmery, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 807.527f, 227.541f });
      polygon.addPoint({ 1105.61f, 265.096f });
      polygon.addPoint({ 951.05f, 448.454f });
      polygon.addPoint({ 708.165f, 408.689f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 770.0f, 160.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::LeftEngine);

      placeLocations.emplace(ggj::PlaceType::LeftEngine, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 2657.87f, 1150.96f });
      polygon.addPoint({ 2918.42f, 1173.05f });
      polygon.addPoint({ 3092.86f, 998.527f });
      polygon.addPoint({ 3035.45f, 888.071f });
      polygon.addPoint({ 2918.42f, 870.398f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 2850.0f, 800.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::LifeSupport);

      placeLocations.emplace(ggj::PlaceType::LifeSupport, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 3549.92f, 870.398f });
      polygon.addPoint({ 3766.31f, 830.633f });
      polygon.addPoint({ 4000.36f, 890.28f });
      polygon.addPoint({ 3806.05f, 1137.7f });
      polygon.addPoint({ 3346.78f, 1093.52f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 3700.0f, 700.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::MainBridge);

      placeLocations.emplace(ggj::PlaceType::MainBridge, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 3552.13f, 859.352f });
      polygon.addPoint({ 3770.73f, 815.169f });
      polygon.addPoint({ 3761.89f, 748.895f });
      polygon.addPoint({ 3598.5f, 731.222f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 3550.0f, 670.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::MidEngine);

      placeLocations.emplace(ggj::PlaceType::MidEngine, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 3050.9f, 888.071f });
      polygon.addPoint({ 3106.1f, 797.496f });
      polygon.addPoint({ 3404.19f, 731.222f });
      polygon.addPoint({ 3585.25f, 740.059f });
      polygon.addPoint({ 3329.12f, 1022.83f });
      polygon.addPoint({ 3108.31f, 991.9f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 2950.0f, 700.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::Navigation);

      placeLocations.emplace(ggj::PlaceType::Navigation, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 679.46f, 421.944f });
      polygon.addPoint({ 924.554f, 463.918f });
      polygon.addPoint({ 739.078f, 693.667f });
      polygon.addPoint({ 480.736f, 653.903f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 600.0f, 350.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::Prison);

      placeLocations.emplace(ggj::PlaceType::Prison, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 1805.56f, 693.667f });
      polygon.addPoint({ 1862.97f, 848.306f });
      polygon.addPoint({ 1635.54f, 1084.68f });
      polygon.addPoint({ 1304.34f, 1038.29f });
      polygon.addPoint({ 1622.3f, 678.203f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 1550.0f, 600.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::Refectory);

      placeLocations.emplace(ggj::PlaceType::Refectory, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 460.864f, 675.994f });
      polygon.addPoint({ 297.468f, 790.869f });
      polygon.addPoint({ 184.858f, 936.672f });
      polygon.addPoint({ 471.904f, 976.436f });
      polygon.addPoint({ 705.957f, 722.386f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 310.0f, 600.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::RightEngine);

      placeLocations.emplace(ggj::PlaceType::RightEngine, location);
    }
    {
      gf::Polygon polygon;
      polygon.addPoint({ 1814.4f, 1135.49f });
      polygon.addPoint({ 1993.25f, 1164.21f });
      polygon.addPoint({ 2344.33f, 790.869f });
      polygon.addPoint({ 2260.42f, 782.032f });
      polygon.addPoint({ 2066.11f, 857.143f });

      assert(polygon.isConvex());

      struct PlaceLocation location;
      location.placeBounds = polygon;
      location.titlePosition = { 2000.0f, 770.0f };
      location.name = ggj::placeTypeString(ggj::PlaceType::Storage);

      placeLocations.emplace(ggj::PlaceType::Storage, location);
    }
  }
}

namespace ggj {
  ShipEntity::ShipEntity(gf::ResourceManager& resources)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_shipTexture(resources.getTexture("image/ship.png"))
  , m_engineTexture(resources.getTexture("image/engine.png"))
  , m_selectedPlace(PlaceType::None)
  , m_drawWarning(false) {
    initializeBounds();

    static constexpr gf::Vector2f EngineTextureSize = { 1072.0f / 6432.0f, 1521.0f / 6084.0f };

    for (int j = 0; j < 4; ++j) {
      for (int i = 0; i < 6; ++i) {
        auto rectTexture = gf::RectF::fromPositionSize({ i * EngineTextureSize.width, j * EngineTextureSize.height }, EngineTextureSize);
        m_engineAnimation.addFrame(m_engineTexture, rectTexture, gf::seconds(1.0f/25.0f));
      }
    }
  }

  PlaceType ShipEntity::getPlaceType(const gf::Vector2f& position) const {
    for (const auto &entry: placeLocations) {
      const auto& location = entry.second;

      auto bounds = location.placeBounds;
      if (bounds.contains(position)) {
        return entry.first;
      }
    }
    return PlaceType::None;
  }

  void ShipEntity::updateMouseCoords(const gf::Vector2i& coords) {
    m_mouseCoords = coords;
  }

  void ShipEntity::selectPlace(PlaceType place) {
    m_selectedPlace = place;
  }

  void ShipEntity::setPlaceState(PlaceType place, bool state) {
    placeLocations.at(place).state = state;
    m_drawWarning = true;
  }

  void ShipEntity::stopDrawWarnings() {
    m_drawWarning = false;
    m_selectedPlace = PlaceType::None;
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

    if (!m_drawWarning) {
      // Display cards
      for (const auto &entry: placeLocations) {
        const auto& key = entry.first;
        const auto& location = entry.second;

        auto drawCursor = [&location, &target, &states, &coordinates](gf::Color4f fillColor, gf::Color4f outlineColor = gf::Color::Black) {
          auto characterSize = coordinates.getRelativeCharacterSize(0.1f);

          gf::CircleShape center(30.0f);
          center.setPosition({ location.placeBounds.getCenter() });
          center.setColor(fillColor);
          center.setOutlineColor(outlineColor);
          center.setOutlineThickness(characterSize * 0.05f);
          center.setAnchor(gf::Anchor::Center);
          target.draw(center, states);
        };

        if (m_selectedPlace == key) {
          drawCursor(gf::Color::White);
        }

        if (!location.placeBounds.contains(m_mouseCoords)) {
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

        drawCursor(gf::Color::White);
      }
      return;
    }

    for (const auto &entry: placeLocations) {
      const auto& location = entry.second;

      auto drawCursor = [&location, &target, &states, &coordinates](gf::Color4f fillColor, gf::Color4f outlineColor = gf::Color::Black) {
        auto characterSize = coordinates.getRelativeCharacterSize(0.1f);

        gf::CircleShape center(30.0f);
        center.setPosition({ location.placeBounds.getCenter() });
        center.setColor(fillColor);
        center.setOutlineColor(outlineColor);
        center.setOutlineThickness(characterSize * 0.05f);
        center.setAnchor(gf::Anchor::Center);
        target.draw(center, states);
      };

      if (location.state) {
        drawCursor(gf::Color::Green);
      }
      else {
        drawCursor(gf::Color::Red);
      }
    }

  }
}
