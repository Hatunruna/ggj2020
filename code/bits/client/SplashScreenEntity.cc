#include "SplashScreenEntity.h"

#include <gf/AnimatedSprite.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>

namespace pem {

  SplashScreenEntity::SplashScreenEntity(std::vector<gf::Ref<gf::Texture>> textures) {
    const gf::Vector2f textureSize = gf::vec(1.0f / 6.0f, 1.0f / 4.0f);

    // Add tileset without empty frame
    for (int i = 0; i < static_cast<int>(textures.size()) - 1; ++i) {
      auto &texture = textures[i];

      m_animation.addTileset(texture, textureSize, { 6, 4 }, 24, gf::seconds(1.0f / 25.0f));
    }

    // Add the last tileset with probably missing frames
    m_animation.addTileset(textures.back(), textureSize, gf::vec(6, 4), 4, gf::seconds(1.0f / 25.0f));
  }

  void SplashScreenEntity::update(gf::Time time) {
    m_animation.update(time);
  }

  void SplashScreenEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);

    gf::AnimatedSprite animation;
    animation.setAnimation(m_animation);
    animation.setPosition(coordinates.getCenter());
    target.draw(animation, states);
  }
}
