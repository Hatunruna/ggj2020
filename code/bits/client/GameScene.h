#ifndef TMPGAME_GAME_SCENE_H
#define TMPGAME_GAME_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>

#include "ViewAdaptator.h"
#include "ShipEntity.h"

namespace ggj {

  struct Scenes;

  class GameScene : public gf::Scene {
  public:
    GameScene(Scenes& scenes, gf::ResourceManager& resources);

  private:
    void doHandleActions(gf::Window& window) override;
    void doProcessEvent(gf::Event &event) override;
    // void doRender(gf::RenderTarget &target, const gf::RenderStates &states);

  private:
    Scenes& m_scenes;

    gf::Action m_escapeAction;

    ViewAdaptator m_adaptator;

    ShipEntity m_ship;

  };

}

#endif // TMPGAME_GAME_SCENE_H
