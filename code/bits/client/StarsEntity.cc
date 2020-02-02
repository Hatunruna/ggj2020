#include "StarsEntity.h"

#include <gf/Color.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Vector.h>

namespace ggj {
  namespace {
    gf::Color4f computeRandomStarColor(gf::Random random, float freq) {
      if (random.computeBernoulli(freq)) {
        return gf::Color::White;
      }

      return {
        random.computeUniformFloat(0.2f, 0.5f),
        0.0f,
        random.computeUniformFloat(0.0f, 0.5f),
        1.0f
      };
    }
  }

  StarsEntity::StarsLayer::StarsLayer(StarsEntity& parent, float velocity, float radius, float whiteFreq)
  : m_parent(&parent)
  , m_velocity(velocity)
  , m_radius(radius)
  , m_whiteFreq(whiteFreq)
  {
    gf::Vector2f topLeftCorner = m_parent->m_bounds.getTopLeft();
    gf::Vector2f bottomRightCorner = m_parent->m_bounds.getBottomRight();

    for (Star& star : m_stars) {
      star.position.x = m_parent->m_random.computeUniformFloat(topLeftCorner.x, bottomRightCorner.x);
      star.position.y = m_parent->m_random.computeUniformFloat(topLeftCorner.y, bottomRightCorner.y);
      star.color = computeRandomStarColor(m_parent->m_random, m_whiteFreq);
    }
  }

  void StarsEntity::StarsLayer::move(float dt) {
    for (Star& star : m_stars) {
      if (star.position.x < m_parent->m_bounds.getTopLeft().x) {
        star.position.x += m_parent->m_bounds.getWidth() * 1.1;
        star.position.y = m_parent->m_random.computeUniformFloat(m_parent->m_bounds.getTopLeft().y, m_parent->m_bounds.getBottomRight().y);
        star.color = computeRandomStarColor(m_parent->m_random, m_whiteFreq);
      }

      star.position.x -= m_velocity * dt;
    }
  }

  std::array<StarsEntity::Star, StarsEntity::StarsLayer::Count>::const_iterator StarsEntity::StarsLayer::begin() const {
    return m_stars.cbegin();
  }

  std::array<StarsEntity::Star, StarsEntity::StarsLayer::Count>::const_iterator StarsEntity::StarsLayer::end() const {
    return m_stars.cend();
  }

  StarsEntity::StarsEntity(const gf::RectF& bounds)
  : gf::Entity(-1)
  , m_bounds(bounds)
  {
    m_layers.emplace_back(*this, 125.0f, 5.0f, 0.25f);
    m_layers.emplace_back(*this, 75.0f, 4.0f, 0.25f);
    m_layers.emplace_back(*this, 20.0f, 3.0f, 1.0f);
  }

  void StarsEntity::update(gf::Time time) {
    float dt = time.asSeconds();

    for (StarsLayer& layer : m_layers) {
      layer.move(dt);
    }
  }

  void StarsEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    for (StarsLayer& layer : m_layers) {
      gf::CircleShape shape(layer.getRadius());
      shape.setAnchor(gf::Anchor::Center);

      for (const Star& star : layer) {
        shape.setPosition(star.position);
        shape.setColor(star.color);
        target.draw(shape, states);
      }
    }
  }
}
