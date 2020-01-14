#ifndef TMPGAME_WAITING_SCENE_H
#define TMPGAME_WAITING_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>

namespace ggj {

  struct Scenes;

  class WaitingScene : public gf::Scene {
  public:
    WaitingScene(Scenes& scenes, gf::ResourceManager& resources);

  private:
    void doHandleActions(gf::Window& window) override;

  private:
    Scenes& m_scenes;

    gf::Action m_escapeAction;
  };

}

#endif // TMPGAME_WAITING_SCENE_H
