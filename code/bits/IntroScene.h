#ifndef TMPGAME_INTRO_SCENE_H
#define TMPGAME_INTRO_SCENE_H

#include <gf/Scene.h>

namespace ggj {

  struct Scenes;

  class IntroScene : public gf::Scene {
  public:

    IntroScene(Scenes& scenes);


  private:
    Scenes& m_scenes;
  };

}

#endif // TMPGAME_INTRO_SCENE_H
