#include "BackgroundEntity.h"

#include <gf/AnimatedSprite.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

#include "common/Constants.h"

#include "Singletons.h"

namespace {
  constexpr gf::Vector2f BackgroundSize = gf::vec(2953.0f, 2045.0f);
  constexpr float BackgroundVelocity = 5.0f / BackgroundSize.x; // 5 px/s

  constexpr int NebulaeCount = 1;
  constexpr float NebulaVelocity = 10.0f; // 10 px/s

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

    // Add nebulae
    for (int i = 0; i < NebulaeCount; ++i) {
      std::string filename = "images/parallax/1_nebulae/nebula_" + std::to_string(i) + ".png";
      m_nebulaeLayer.emplace_back(resources.getTexture(filename));
      m_nebulaeLayer.back().texture.setSmooth();
    }
    chooseNebula();

    // Add stars layer
    for (unsigned i = 0; i < StarsVelocities.size(); ++i) {
      std::string filename = "images/parallax/0_stars/stars_" + std::to_string(i) + ".png";
      m_starsLayer.emplace_back(resources.getTexture(filename));
      m_starsLayer.back().texture.setSmooth();
      m_starsLayer.back().texture.setRepeated();
    }
  }

  void BackgroundEntity::update(gf::Time time) {
    // Update background postion
    m_backgroundLayer.position.x += time.asSeconds() * BackgroundVelocity;

    // Update nebula layer
    m_nebulaeLayer[m_selectedNebula].position.x -= time.asSeconds() * NebulaVelocity;
    if (m_nebulaeLayer[m_selectedNebula].position.x < -WorldSize.width) {
      chooseNebula();
    }

    // Update stars layer
    for (unsigned i = 0; i < StarsVelocities.size(); ++i) {
      auto &starsLayer = m_starsLayer[i];
      starsLayer.position.x += time.asSeconds() * StarsVelocities[i];
    }
  }

  void BackgroundEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);

    // // Draw background
    gf::Sprite background;
    background.setTexture(m_backgroundLayer.texture);
    background.setTextureRect(gf::RectF::fromPositionSize(m_backgroundLayer.position, gf::vec(1.0f,1.0f)));
    background.setPosition(gf::vec(0.0f, 0.0f));
    target.draw(background, states);

    // Draw nebula
    const auto& nebulaLayer = m_nebulaeLayer[m_selectedNebula];
    gf::Sprite nebula;
    nebula.setTexture(nebulaLayer.texture);
    // nebula.setTextureRect(gf::RectF::fromPositionSize(nebulaLayer.position, normalizedNebulaTextureRectSize));
    nebula.setPosition(nebulaLayer.position);
    nebula.setAnchor(gf::Anchor::CenterLeft);
    target.draw(nebula, states);

    // Draw stars layer
    for (const auto& starsLayer: m_starsLayer) {
      gf::Sprite stars;
      stars.setTexture(starsLayer.texture);
      stars.setTextureRect(gf::RectF::fromPositionSize(starsLayer.position, gf::vec(1.0f,1.0f)));
      stars.setPosition(gf::vec(0.0f, 0.0f));
      target.draw(stars, states);
    }
  }

  void BackgroundEntity::chooseNebula() {
    m_selectedNebula = gRandom().computeUniformInteger<int>(0, m_nebulaeLayer.size() - 1);
    // Reset position outside of screen
    m_nebulaeLayer[m_selectedNebula].position = gf::vec(WorldSize.width + 1.0f, WorldSize.height * 0.5f);
  }
}
