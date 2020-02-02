#ifndef TMPGAME_STARS_ENTITY_H
#define TMPGAME_STARS_ENTITY_H

#include <array>
#include <vector>

#include <gf/Entity.h>
#include <gf/Random.h>
#include <gf/Rect.h>

namespace ggj {
  class StarsEntity : public gf::Entity {
  public:
    StarsEntity(const gf::RectF& bounds);

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    struct Star {
      gf::Vector2f position;
      gf::Color4f color;
    };

    class StarsLayer {
    private:
      static constexpr std::size_t Count = 100;

    public:
      StarsLayer(StarsEntity& parent, float velocity, float radius, float whiteFreq);
      void move(float dt);

      std::array<Star, Count>::const_iterator begin() const;
      std::array<Star, Count>::const_iterator end() const;

      float getRadius() const {
        return m_radius;
      }

    private:
      StarsEntity* m_parent;
      float m_velocity;
      float m_radius;
      float m_whiteFreq;

      std::array<Star, Count> m_stars;
    };

    gf::RectF m_bounds;
    gf::Random m_random;
    std::vector<StarsLayer> m_layers;
  };
}

#endif /* ifndef TMPGAME_STARS_ENTITY_H */
