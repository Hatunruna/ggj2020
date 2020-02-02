#ifndef TMPGAME_BACKGROUND_ENTITY
#define TMPGAME_BACKGROUND_ENTITY

#include <gf/Entity.h>
#include <gf/ResourceManager.h>

#include "StarsEntity.h"

namespace ggj {

  class BackgroundEntity: public gf::Entity {
  public:
    BackgroundEntity(gf::ResourceManager& resources, bool displayShip = true);

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Texture &m_texture;
    StarsEntity m_stars;
    bool m_displayShip;
  };

}


#endif // TMPGAME_BACKGROUND_ENTITY
