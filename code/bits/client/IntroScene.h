#ifndef PEM_INTRO_SCENE_H
#define PEM_INTRO_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>

#include "BackgroundEntity.h"
#include "MenuEntity.h"

namespace pem {

  struct Scenes;

  class IntroScene : public gf::Scene {
  public:
    IntroScene(Scenes& scenes, gf::ResourceManager& resources);

  private:
    void doProcessEvent(gf::Event& event) override;
    void doHandleActions(gf::Window& window) override;
    void doUpdate(gf::Time time) override;

  private:
    Scenes& m_scenes;

    gf::Action m_escapeAction;

    MenuChoice m_choice;
    MenuEntity m_menu;

    BackgroundEntity m_backgorund;
  };

}

#endif // PEM_INTRO_SCENE_H
