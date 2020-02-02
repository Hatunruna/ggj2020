#ifndef TMPGAME_BACKGROUND_ENTITY
#define TMPGAME_BACKGROUND_ENTITY

#include <gf/Entity.h>
#include <gf/ResourceManager.h>

namespace ggj {

  class BackgroundEntity: public gf::Entity {
  public:
    BackgroundEntity(gf::ResourceManager& resources);

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Texture &m_texture;
  };

}


#endif // TMPGAME_BACKGROUND_ENTITY
