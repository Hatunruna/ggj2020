#ifndef TMPGAME_SINGLETONS_H
#define TMPGAME_SINGLETONS_H

#include <gf/Singleton.h>

#include "ResourceManager.h"

namespace ggj {

  extern gf::Singleton<ResourceManager> gResourceManager;
  extern sf::Sound gBackgroundMusic;

}

#endif // TMPGAME_SINGLETONS_H
