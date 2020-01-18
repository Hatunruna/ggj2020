#include "RoomScene.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>

#include "Scenes.h"

namespace ggj {

  RoomScene::RoomScene(Scenes& scenes, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_ui(resources.getFont("DejaVuSans.ttf"))
  {
  }

  void RoomScene::doProcessEvent(gf::Event& event) {
    m_ui.processEvent(event);
  }

  void RoomScene::doUpdate(gf::Time time) {
  }

  void RoomScene::doRender(gf::RenderTarget& target) {
    gf::Coordinates coords(target);

    // UI

    if (m_ui.begin("Room", gf::RectF::fromCenterSize(coords.getCenter(), { 600.0f, 400.0f }), gf::UIWindow::Border | gf::UIWindow::Title)) {




      m_ui.layoutRowDynamic(25.0f, 1);

      if (m_ui.buttonLabel("Back")) {
        m_scenes.replaceScene(m_scenes.lobby);
      }
    }

    m_ui.end();

    // Display

    renderWorldEntities(target);
    renderHudEntities(target);
    target.draw(m_ui);
  }

}
