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
    gf::Texture &m_texture;
    // gf::Texture &m_engineTexture;
    // gf::Animation m_engineAnimation;
    StarsEntity m_stars;
    bool m_displayShip;
  };

}


#endif // PEM_BACKGROUND_ENTITY
