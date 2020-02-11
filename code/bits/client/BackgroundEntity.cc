#include "BackgroundEntity.h"

#include <gf/AnimatedSprite.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

namespace pem {
  BackgroundEntity::BackgroundEntity(gf::ResourceManager& resources, bool displayShip)
  : m_texture(resources.getTexture("images/ship.png"))
  // , m_engineTexture(resources.getTexture("image/engine.png"))
  , m_stars(gf::RectF::fromMinMax({-3000.0f, -1000.0f}, {6000.0f, 3000.0f}))
  , m_displayShip(displayShip) {
    // static constexpr gf::Vector2f EngineTextureSize = { 1072.0f / 6432.0f, 1521.0f / 6084.0f };
    // for (int j = 0; j < 4; ++j) {
    //   for (int i = 0; i < 6; ++i) {
    //     auto rectTexture = gf::RectF::fromPositionSize({ i * EngineTextureSize.width, j * EngineTextureSize.height }, EngineTextureSize);
    //     m_engineAnimation.addFrame(m_engineTexture, rectTexture, gf::seconds(1.0f/25.0f));
    //   }
    // }
  }

  void BackgroundEntity::update(gf::Time time) {
    m_stars.update(time);
    // m_engineAnimation.update(time);
  }

  void BackgroundEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    m_stars.render(target, states);

    if (!m_displayShip) {
      return;
    }

    static constexpr gf::Vector2f TextureSize = { 4030.0f, 1521.0f };
    static constexpr gf::Vector2f TextureEngineSize = { 1072.0f, 1521.0f };
    static constexpr float RatioTexture = TextureSize.x / TextureSize.y;
    static constexpr float RatioEngineTexture = TextureEngineSize.x / TextureEngineSize.y;

    gf::Coordinates coordinates(target);
    auto screenSize = coordinates.getRelativeSize({1.0f, 1.0f});

    // {
    //   float newHeight = screenSize.width / RatioEngineTexture;
    //   gf::Vector2f newSize = { screenSize.width, newHeight };
    //   gf::Vector2f scaleFactors = newSize / TextureEngineSize;

    //   gf::AnimatedSprite animation;
    //   animation.setAnimation(m_engineAnimation);
    //   animation.setPosition({ 625.0f, 0.0f });
    //   animation.scale(scaleFactors);
    //   animation.setAnchor(gf::Anchor::TopRight);
    //   target.draw(animation, states);
    // }


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
