#ifndef TMPGAME_SCENES_H
#define TMPGAME_SCENES_H

#include <gf/SceneManager.h>

#include "IntroScene.h"

namespace ggj {

  struct Scenes : gf::SceneManager {
    Scenes();

    IntroScene intro;

  };

}

#endif // TMPGAME_SCENES_H
