#include "IntroScene.h"

#include "Constants.h"
#include "Scenes.h"

namespace ggj {

  IntroScene::IntroScene(Scenes& scenes, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_menu(resources)
  {
    addHudEntity(m_menu);
  }

}
