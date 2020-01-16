#ifndef TMPGAME_CONNECTION_SCENE_H
#define TMPGAME_CONNECTION_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>
#include <gf/UI.h>

#include "MenuEntity.h"

namespace ggj {

  struct Scenes;

  class ConnectionScene : public gf::Scene {
  public:

    ConnectionScene(Scenes& scenes, gf::ResourceManager& resources);

  private:
    void doProcessEvent(gf::Event& event) override;
//     void doHandleActions(gf::Window& window) override;
    void doUpdate(gf::Time time) override;

    void doRender(gf::RenderTarget& target) override;


  private:
    Scenes& m_scenes;
    gf::UI m_ui;
    gf::UICharBuffer m_hostnameBuffer;
    gf::UICharBuffer m_nameBuffer;
  };

}

#endif // TMPGAME_INTRO_SCENE_H
