#include "IntroScene.h"

#include "Constants.h"
#include "Scenes.h"

namespace ggj {

  IntroScene::IntroScene(Scenes& scenes)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  {
  }

}
