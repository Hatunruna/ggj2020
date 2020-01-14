#ifndef TMPGAME_SCENES_H
#define TMPGAME_SCENES_H

#include <gf/SceneManager.h>
#include <gf/ResourceManager.h>

#include "IntroScene.h"
#include "WaitingScene.h"

namespace ggj {

  struct Scenes : gf::SceneManager {
    Scenes(gf::Path searchDir);

    gf::ResourceManager resources;

    IntroScene intro;
    WaitingScene waiting;
  };

}

#endif // TMPGAME_SCENES_H
