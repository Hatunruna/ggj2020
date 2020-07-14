#include "BackgroundEntity.h"

#include <gf/AnimatedSprite.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

namespace {
  constexpr gf::Vector2f BackgroundSize = gf::vec(2953.0f, 2045.0f);
  constexpr float BackgroundVelocity = 20.0f / BackgroundSize.x; // 20 px/s
}

namespace pem {
  BackgroundEntity::BackgroundEntity(gf::ResourceManager& resources, bool displayShip)
  : m_backgroundTexture(resources.getTexture("images/parallax/background.png"))
  , m_backgroundPosition(gf::vec(0.0f, 0.5f)) {
    m_backgroundTexture.setSmooth();
    m_backgroundTexture.setRepeated();
  }

  void BackgroundEntity::update(gf::Time time) {
    m_backgroundPosition.x += time.asSeconds() * BackgroundVelocity;
  }

  void BackgroundEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);
    auto screenSize = coordinates.getRelativeSize({1.0f, 1.0f});

    // Compute background texture normalized size
    gf::Vector2f normalizedTextureRectSize = screenSize / BackgroundSize;

    gf::Sprite background;
    background.setTexture(m_backgroundTexture);
    background.setTextureRect(gf::RectF::fromPositionSize(m_backgroundPosition, normalizedTextureRectSize));
    background.setPosition(gf::vec(0.0f, 0.0f));
    target.draw(background, states);
  }
}
