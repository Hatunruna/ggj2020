#ifndef PEM_CREDITS_SCENE_H
#define PEM_CREDITS_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>

#include "BackgroundEntity.h"

namespace pem {

  struct Scenes;

  class CreditsScene : public gf::Scene {
  public:
    CreditsScene(Scenes& scenes, gf::ResourceManager& resources);

  private:
    void doHandleActions(gf::Window& window) override;
    void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    Scenes& m_scenes;

    gf::Action m_escapeAction;
    gf::Font &m_font;

    BackgroundEntity m_backgorund;
  };

}

#endif // PEM_CREDITS_SCENE_H
