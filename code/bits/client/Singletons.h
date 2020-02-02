#ifndef TMPGAME_SINGLETONS_H
#define TMPGAME_SINGLETONS_H

#include <gf/Singleton.h>

#include "ResourceManager.h"

namespace ggj {

  static constexpr float BackgroundAmbiantVolume = 10.0f;
  static constexpr float FxsVolume = 80.0f;

  extern gf::Singleton<ResourceManager> gResourceManager;
  extern sf::Sound gBackgroundMusic;

}

#endif // TMPGAME_SINGLETONS_H
