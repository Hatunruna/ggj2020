#ifndef TMPGAME_ROOM_SCENE_H
#define TMPGAME_ROOM_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>
#include <gf/UI.h>

namespace ggj {

  struct Scenes;

  class RoomScene : public gf::Scene {
  public:

    RoomScene(Scenes& scenes, gf::ResourceManager& resources);

  private:
    void doProcessEvent(gf::Event& event) override;
    void doUpdate(gf::Time time) override;
    void doRender(gf::RenderTarget& target) override;

  private:
    Scenes& m_scenes;
    gf::UI m_ui;
  };

}

#endif // TMPGAME_ROOM_SCENE_H
