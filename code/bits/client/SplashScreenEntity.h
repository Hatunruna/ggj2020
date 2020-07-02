#ifndef PEM_SPLASH_SCREEN_ENTITY_H
#define PEM_SPLASH_SCREEN_ENTITY_H

#include <atomic>

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/Ref.h>
#include <gf/ResourceManager.h>

namespace pem {

  class SplashScreenEntity: public gf::Entity {
  public:
    SplashScreenEntity(gf::Texture& tempTexture);

    void loadAnimation(std::vector<gf::Ref<gf::Texture>> textures);

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Texture& m_tempTexture;
    gf::Animation m_animation;
    std::atomic_bool m_animationLoaded;
  };

} // namespace pem

#endif // PEM_SPLASH_SCREEN_ENTITY_H