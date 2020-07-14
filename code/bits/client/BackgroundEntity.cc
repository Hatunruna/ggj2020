#include "BackgroundEntity.h"

#include <gf/AnimatedSprite.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

namespace {
  constexpr gf::Vector2f BackgroundSize = gf::vec(2953.0f, 2045.0f);

  constexpr float BackgroundVelocity = 5.0f / BackgroundSize.x; // 5 px/s
  constexpr std::array<float, 3> StarsVelocities = {
    15.0f / BackgroundSize.x, // stars_1.png -> 15 px/s
    20.0f / BackgroundSize.x, // stars_2.png -> 20 px/s
    25.0f / BackgroundSize.x  // stars_3.png -> 25 px/s
  };
}

namespace pem {
  BackgroundEntity::BackgroundEntity(gf::ResourceManager& resources, bool displayShip)
  : m_backgroundLayer(resources.getTexture("images/parallax/background.png")) {
    m_backgroundLayer.texture.setSmooth();
    m_backgroundLayer.texture.setRepeated();

    // Add stars layer
    for (unsigned i = 0; i < StarsVelocities.size(); ++i) {
      std::string filename = "images/parallax/0_stars/stars_" + std::to_string(i) + ".png";
      m_starsLayers.emplace_back(resources.getTexture(filename));
      m_starsLayers.back().texture.setSmooth();
      m_starsLayers.back().texture.setRepeated();
    }
  }

  void BackgroundEntity::update(gf::Time time) {
    m_backgroundLayer.position.x += time.asSeconds() * BackgroundVelocity;

    // Update stars layers
    for (unsigned i = 0; i < StarsVelocities.size(); ++i) {
      auto &starsLayer = m_starsLayers[i];
      starsLayer.position.x += time.asSeconds() * StarsVelocities[i];
    }
  }

  void BackgroundEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);
    auto screenSize = coordinates.getRelativeSize({1.0f, 1.0f});

    // Compute background texture normalized size
    gf::Vector2f normalizedTextureRectSize = screenSize / BackgroundSize;

    gf::Sprite background;
    background.setTexture(m_backgroundLayer.texture);
    background.setTextureRect(gf::RectF::fromPositionSize(m_backgroundLayer.position, normalizedTextureRectSize));
    background.setPosition(gf::vec(0.0f, 0.0f));
    target.draw(background, states);

    // Draw stars layers
    for (const auto& starsLayer: m_starsLayers) {
      gf::Sprite stars;
      stars.setTexture(starsLayer.texture);
      stars.setTextureRect(gf::RectF::fromPositionSize(starsLayer.position, normalizedTextureRectSize));
      stars.setPosition(gf::vec(0.0f, 0.0f));
      target.draw(stars, states);
    }
  }
}
