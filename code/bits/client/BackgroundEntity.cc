#include "BackgroundEntity.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

namespace ggj {
  BackgroundEntity::BackgroundEntity(gf::ResourceManager& resources, bool displayShip)
  : m_texture(resources.getTexture("image/ship.png"))
  , m_stars(gf::RectF::fromMinMax({-3000.0f, -1000.0f}, {6000.0f, 3000.0f}))
  , m_displayShip(displayShip) {

  }

  void BackgroundEntity::update(gf::Time time) {
    m_stars.update(time);
  }

  void BackgroundEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    m_stars.render(target, states);

    if (!m_displayShip) {
      return;
    }

    static constexpr gf::Vector2f TextureSize = { 4030.0f, 1521.0f };
    static constexpr float RatioTexture = TextureSize.x / TextureSize.y;

    gf::Coordinates coordinates(target);
    auto screenSize = coordinates.getRelativeSize({1.0f, 1.0f});

    float newHeight = screenSize.width / RatioTexture;
    gf::Vector2f newSize = { screenSize.width, newHeight };
    gf::Vector2f scaleFactors = newSize / TextureSize;

    gf::Sprite ship;
    ship.setTexture(m_texture);
    ship.setPosition(coordinates.getCenter());
    ship.scale(scaleFactors);
    ship.setAnchor(gf::Anchor::Center);
    target.draw(ship, states);
  }
}
