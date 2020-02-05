#ifndef PEM_SINGLETONS_H
#define PEM_SINGLETONS_H

#include <gf/Singleton.h>

#include "ResourceManager.h"

namespace pem {

  static constexpr float BackgroundAmbiantVolume = 10.0f;
  static constexpr float FxsVolume = 80.0f;

  extern gf::Singleton<ResourceManager> gResourceManager;
  extern sf::Sound gBackgroundMusic;

}

#endif // PEM_SINGLETONS_H
