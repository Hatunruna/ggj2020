#include "SplashScreenEntity.h"

#include <gf/AnimatedSprite.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>

static constexpr gf::Vector2i SplashScreenSize = {526, 1008};

namespace pem {

  SplashScreenEntity::SplashScreenEntity(gf::Texture& tempTexture)
  : m_tempTexture(tempTexture)
  , m_animationLoaded(false) {

  }

  void SplashScreenEntity::loadAnimation(std::vector<gf::Ref<gf::Texture>> textures) {
    const gf::Vector2f textureSize = gf::vec(1.0f / 6.0f, 1.0f / 4.0f);

    // Add tileset without empty frame
    for (int i = 0; i < static_cast<int>(textures.size()) - 1; ++i) {
      auto &texture = textures[i];

      m_animation.addTileset(texture, textureSize, { 6, 4 }, 24, gf::seconds(1.0f / 25.0f));
    }

    // Add the last tileset with probably missing frames
    m_animation.addTileset(textures.back(), textureSize, { 6, 4 }, 4, gf::seconds(1.0f / 25.0f));

    m_animationLoaded = true;
  }

  void SplashScreenEntity::update(gf::Time time) {
    if (!m_animationLoaded) {
      return;
    }

    m_animation.update(time);
  }

  void SplashScreenEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);

    if (!m_animationLoaded) {
      gf::Sprite sprite(m_tempTexture);
      if (target.getSize().height < SplashScreenSize.height) {
        float scaleFactor = static_cast<float>(target.getSize().height) / static_cast<float>(SplashScreenSize.height);
        sprite.scale(scaleFactor);
      }
      sprite.setAnchor(gf::Anchor::Center);
      sprite.setPosition(coordinates.getCenter());
      target.draw(sprite, states);
    }
    else {
      gf::AnimatedSprite animation;
      animation.setAnimation(m_animation);
      if (target.getSize().height < SplashScreenSize.height) {
        float scaleFactor = static_cast<float>(target.getSize().height) / static_cast<float>(SplashScreenSize.height);
        animation.scale(scaleFactor);
      }
      animation.setAnchor(gf::Anchor::Center);
      animation.setPosition(coordinates.getCenter());
      target.draw(animation, states);
    }
  }
}
