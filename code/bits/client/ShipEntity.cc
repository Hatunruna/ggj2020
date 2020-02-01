#include "ShipEntity.h"

#include <gf/Curves.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

#include "common/PemTypes.h"

namespace {
  struct PlaceLocation {
    gf::RectF placeBounds;
    gf::Vector2f arrowStart;
    gf::Vector2f arrowEnd;
    gf::Vector2f titlePosition;
    std::string name;
  };

  std::map<ggj::PlaceType, PlaceLocation> placeLocations = {
    { ggj::PlaceType::Armory,              { gf::RectF::fromPositionSize({ 250.0f, 250.0f }, { 50.0f, 50.0f }), { 275.0f, 240.0f }, { 275.0f, 100.0f }, { 275.0f, 90.0f }, ggj::placeTypeString(ggj::PlaceType::Armory)}},
    // { ggj::PlaceType::Bathroom,            { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::Bathroom)}},
    // { ggj::PlaceType::CommunicationCenter, { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::CommunicationCenter)}},
    // { ggj::PlaceType::Dormitory,           { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::Dormitory)}},
    // { ggj::PlaceType::GreenHouse,          { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::GreenHouse)}},
    // { ggj::PlaceType::Infirmery,           { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::Infirmery)}},
    // { ggj::PlaceType::LeftEngine,          { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::LeftEngine)}},
    // { ggj::PlaceType::LifeSupport,         { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::LifeSupport)}},
    // { ggj::PlaceType::MainBridge,          { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::MainBridge)}},
    // { ggj::PlaceType::MidEngine,           { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::MidEngine)}},
    // { ggj::PlaceType::Navigation,          { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::Navigation)}},
    // { ggj::PlaceType::Prison,              { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::Prison)}},
    // { ggj::PlaceType::Refectory,           { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::Refectory)}},
    // { ggj::PlaceType::RightEngine,         { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::RightEngine)}},
    // { ggj::PlaceType::Storage,             { gf::RectF::fromPositionSize({ 100.0f, 100.0f }, { 100.0f, 100.0f }), { 0.0f, 0.0f }, { 100.0f, 100.0f }, { 100.0f, 100.0f }, ggj::placeTypeString(ggj::PlaceType::Storage)}},
  };
}

namespace ggj {
  ShipEntity::ShipEntity(gf::ResourceManager& resources)
  : m_font(resources.getFont("DejaVuSans.ttf")) {
    m_rect.setColor(gf::Color::White);
    m_rect.setSize({6000.0f, 500.0f});
    m_rect.setPosition({0.f, 500.0f});
    m_rect.setAnchor(gf::Anchor::CenterLeft);
  }

  void ShipEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);
    target.draw(m_rect, states);

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

      gf::Line arrow(location.arrowStart, location.arrowEnd);
      arrow.setWidth(2.0f);
      arrow.setColor(gf::Color::Violet);
      target.draw(arrow, states);

      gf::Text text(location.name, m_font);
      text.setCharacterSize(coordinates.getRelativeCharacterSize(0.1f));
      text.setColor(gf::Color::Violet);
      text.setOutlineColor(gf::Color::White);
      text.setOutlineThickness(2.0f);
      text.setPosition(location.titlePosition);
      text.setAnchor(gf::Anchor::BottomCenter);
      target.draw(text, states);
    }
  }
}
