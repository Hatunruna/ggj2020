#ifndef PEM_BACKGROUND_ENTITY
#define PEM_BACKGROUND_ENTITY

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/ResourceManager.h>

#include "StarsEntity.h"

namespace pem {
  struct ParallaxEntities {
    ParallaxEntities(gf::Texture& texture)
    : texture(texture)
    , position(gf::vec(0.0f, 0.0f)) {
    }

    gf::Texture& texture;
    gf::Vector2f position;
  };

  class BackgroundEntity: public gf::Entity {
  public:
    BackgroundEntity(gf::ResourceManager& resources, bool displayShip = true);

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  public:
    void chooseNebula();

  private:
    // Last layer
    ParallaxEntities m_backgroundLayer;

    // Nebulae layer
    std::vector<ParallaxEntities> m_nebulaeLayer;
    int m_selectedNebula;

    // Star layer
    std::vector<ParallaxEntities> m_starsLayer;
  };

}


#endif // PEM_BACKGROUND_ENTITY
