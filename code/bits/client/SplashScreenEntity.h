#ifndef PEM_SPLASH_SCREEN_ENTITY_H
#define PEM_SPLASH_SCREEN_ENTITY_H

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/Ref.h>
#include <gf/ResourceManager.h>

namespace pem {

  class SplashScreenEntity: public gf::Entity {
  public:
    SplashScreenEntity(std::vector<gf::Ref<gf::Texture>> textures);

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Animation m_animation;
  };

} // namespace pem

#endif // PEM_SPLASH_SCREEN_ENTITY_H