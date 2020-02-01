#include "ShipEntity.h"

#include <gf/Curves.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

#include "common/PemTypes.h"

namespace {
  struct PlaceLocation {
    gf::RectF placeBounds;
    gf::Vector2f titlePosition;
    std::string name;
  };

  std::map<ggj::PlaceType, PlaceLocation> placeLocations = {
    { ggj::PlaceType::Armory,              { gf::RectF::fromPositionSize({ 2500, 425.0f }, { 100.0f, 100.0f }), { 2450.0f, 350.0f }, ggj::placeTypeString(ggj::PlaceType::Armory)}},
    { ggj::PlaceType::Bathroom,            { gf::RectF::fromPositionSize({ 2250.0f, 445.0f }, { 100.0f, 100.0f }), { 2200.0f, 370.0f }, ggj::placeTypeString(ggj::PlaceType::Bathroom)}},
    { ggj::PlaceType::CommunicationCenter, { gf::RectF::fromPositionSize({ 3215.0f, 565.0f }, { 100.0f, 100.0f }), { 3165.0f, 490.0f }, ggj::placeTypeString(ggj::PlaceType::CommunicationCenter)}},
    { ggj::PlaceType::Dormitory,           { gf::RectF::fromPositionSize({ 1920.0f, 350.0f }, { 100.0f, 100.0f }), { 1870.0f, 275.0f }, ggj::placeTypeString(ggj::PlaceType::Dormitory)}},
    { ggj::PlaceType::GreenHouse,          { gf::RectF::fromPositionSize({ 2710.0f, 425.0f }, { 100.0f, 100.0f }), { 2660.0f, 350.0f }, ggj::placeTypeString(ggj::PlaceType::GreenHouse)}},
    { ggj::PlaceType::Infirmery,           { gf::RectF::fromPositionSize({ 1870.0f, 845.0f }, { 100.0f, 100.0f }), { 1820.0f, 770.0f }, ggj::placeTypeString(ggj::PlaceType::Infirmery)}},
    { ggj::PlaceType::LeftEngine,          { gf::RectF::fromPositionSize({ 820.0f, 235.0f }, { 100.0f, 100.0f }), { 770.0f, 160.0f }, ggj::placeTypeString(ggj::PlaceType::LeftEngine)}},
    { ggj::PlaceType::LifeSupport,         { gf::RectF::fromPositionSize({ 2900.0f, 875.0f }, { 100.0f, 100.0f }), { 2850.0f, 800.0f }, ggj::placeTypeString(ggj::PlaceType::LifeSupport)}},
    { ggj::PlaceType::MainBridge,           { gf::RectF::fromPositionSize({ 3750.0f, 775.0f }, { 100.0f, 100.0f }), { 3700.0f, 700.0f }, ggj::placeTypeString(ggj::PlaceType::MainBridge)}},
    { ggj::PlaceType::MidEngine,          { gf::RectF::fromPositionSize({ 3600.0f, 670.0f }, { 100.0f, 100.0f }), { 3550.0f, 670.0f }, ggj::placeTypeString(ggj::PlaceType::MidEngine)}},
    { ggj::PlaceType::Navigation,          { gf::RectF::fromPositionSize({ 3000.0f, 775.0f }, { 100.0f, 100.0f }), { 2950.0f, 700.0f }, ggj::placeTypeString(ggj::PlaceType::Navigation)}},
    { ggj::PlaceType::Prison,              { gf::RectF::fromPositionSize({ 650.0f, 425.0f }, { 100.0f, 100.0f }), { 600.0f, 350.0f }, ggj::placeTypeString(ggj::PlaceType::Prison)}},
    { ggj::PlaceType::Refectory,           { gf::RectF::fromPositionSize({ 1600.0f, 675.0f }, { 100.0f, 100.0f }), { 1550.0f, 600.0f }, ggj::placeTypeString(ggj::PlaceType::Refectory)}},
    { ggj::PlaceType::RightEngine,         { gf::RectF::fromPositionSize({ 360.0f, 675.0f }, { 100.0f, 100.0f }), { 310.0f, 600.0f }, ggj::placeTypeString(ggj::PlaceType::RightEngine)}},
    { ggj::PlaceType::Storage,             { gf::RectF::fromPositionSize({ 2050.0f, 845.0f }, { 100.0f, 100.0f }), { 2000.0f, 770.0f }, ggj::placeTypeString(ggj::PlaceType::Storage)}},
  };
}

namespace ggj {
  ShipEntity::ShipEntity(gf::ResourceManager& resources)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_shipTexture(resources.getTexture("image/ship.png")) {

  }

  void ShipEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);
    gf::Sprite ship(m_shipTexture);
    ship.setPosition({ 0.0f, 0.0f });
    target.draw(ship, states);

    // Display cards
    for (const auto &entry: placeLocations) {
      const auto& location = entry.second;

      auto bounds = location.placeBounds;
      gf::RectangleShape rectangle;
      rectangle.setColor(gf::Color::Transparent);
      rectangle.setOutlineColor(gf::Color::Violet);
      rectangle.setOutlineThickness(2.0f);
      rectangle.setSize(bounds.getSize());
      rectangle.setPosition(bounds.getPosition());
      target.draw(rectangle, states);

      gf::Text text(location.name, m_font);
      text.setCharacterSize(coordinates.getRelativeCharacterSize(0.1f));
      text.setColor(gf::Color::Violet);
      text.setOutlineColor(gf::Color::White);
      text.setOutlineThickness(2.0f);
      text.setPosition(location.titlePosition);
      text.setAnchor(gf::Anchor::BottomLeft);
      target.draw(text, states);
    }
  }

  bool ShipEntity::getPlaceType(const gf::Vector2f& position, PlaceType& res) const {
    for (const auto &entry: placeLocations) {
      const auto& location = entry.second;

      auto bounds = location.placeBounds;
      if (bounds.contains(position)) {
        res = entry.first;
        return true;
      }
    }
    return false;
  }
}
