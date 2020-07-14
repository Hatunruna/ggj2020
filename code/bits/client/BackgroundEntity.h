#ifndef PEM_BACKGROUND_ENTITY
#define PEM_BACKGROUND_ENTITY

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/ResourceManager.h>

#include "StarsEntity.h"

namespace pem {
  struct ParallaxLayer {
    ParallaxLayer(gf::Texture& texture)
    : texture(texture)
    , position(gf::vec(0.0f, 0.5f)) {
    }

    gf::Texture& texture;
    gf::Vector2f position;
  };

  class BackgroundEntity: public gf::Entity {
  public:
    BackgroundEntity(gf::ResourceManager& resources, bool displayShip = true);

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    ParallaxLayer m_backgroundLayer;
    std::vector<ParallaxLayer> m_starsLayers;
  };

}


#endif // PEM_BACKGROUND_ENTITY
