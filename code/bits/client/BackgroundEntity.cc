#include "BackgroundEntity.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

namespace ggj {
  BackgroundEntity::BackgroundEntity(gf::ResourceManager& resources)
  : m_texture(resources.getTexture("image/ship.png")) {

  }

  void BackgroundEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
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

    gf::RectangleShape alpha;
    alpha.setSize(screenSize);
    alpha.setColor(gf::Color::Opaque());
    alpha.setPosition(coordinates.getCenter());
    alpha.setAnchor(gf::Anchor::Center);
    target.draw(alpha, states);
  }
}
