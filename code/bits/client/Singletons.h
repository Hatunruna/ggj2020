#ifndef TMPGAME_SINGLETONS_H
#define TMPGAME_SINGLETONS_H

#include <gf/Singleton.h>

#include "ResourceManager.h"

namespace ggj {

  static constexpr float BackgroundAmbiantVolume = 0.0f;
  static constexpr float FxsVolume = 0.0f;

  extern gf::Singleton<ResourceManager> gResourceManager;
  extern sf::Sound gBackgroundMusic;

}

#endif // TMPGAME_SINGLETONS_H
