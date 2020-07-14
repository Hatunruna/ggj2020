#ifndef PEM_BACKGROUND_ENTITY
#define PEM_BACKGROUND_ENTITY

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/ResourceManager.h>

#include "StarsEntity.h"

namespace pem {

  class BackgroundEntity: public gf::Entity {
  public:
    BackgroundEntity(gf::ResourceManager& resources, bool displayShip = true);

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Texture& m_backgroundTexture;
    gf::Vector2f m_backgroundPosition;
  };

}


#endif // PEM_BACKGROUND_ENTITY
