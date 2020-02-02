#ifndef TMPGAME_CONNECTION_SCENE_H
#define TMPGAME_CONNECTION_SCENE_H

#include <gf/Scene.h>
#include <gf/StaticString.h>

#include "BackgroundEntity.h"
#include "ClientNetwork.h"

namespace ggj {

  struct Scenes;

  class ConnectionScene : public gf::Scene {
  public:
    ConnectionScene(Scenes& scenes, gf::ResourceManager& resources, ClientNetwork& network);

  private:
    void doProcessEvent(gf::Event& event) override;
//     void doHandleActions(gf::Window& window) override;
    void doUpdate(gf::Time time) override;

    void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;


  private:
    Scenes& m_scenes;
    ClientNetwork& m_network;
    gf::StaticString<255> m_hostnameBuffer;
    gf::StaticString<255> m_nameBuffer;
    bool m_connectionAsked;

    BackgroundEntity m_backgorund;
  };

}

#endif // TMPGAME_INTRO_SCENE_H
