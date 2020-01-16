#include "ConnectionScene.h"

#include <gf/Log.h>

#include "Constants.h"
#include "Scenes.h"

namespace ggj {

  ConnectionScene::ConnectionScene(Scenes& scenes, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_ui(resources.getFont("DejaVuSans.ttf"))
  , m_hostnameBuffer(128)
  , m_nameBuffer(MaxPlayerNameLength + 1)
  {
    m_hostnameBuffer.append("localhost");
    m_nameBuffer.append("toto");
  }

  void ConnectionScene::doProcessEvent(gf::Event& event) {
    m_ui.processEvent(event);
  }

  void ConnectionScene::doUpdate(gf::Time time) {
    auto editRatios = { 0.25f, 0.75f };
    auto buttonRatios = { 0.5f, 0.5f };

    if (m_ui.begin("Connect", gf::RectF::fromPositionSize({ 20.0f, 20.0f }, { 400.0f, 150.0f }), gf::UIWindow::Border | gf::UIWindow::Movable | gf::UIWindow::Title)) {
      m_ui.layoutRow(gf::UILayout::Dynamic, 25.0f, gf::array(editRatios.begin(), editRatios.size()));

      m_ui.label("Hostname:");
      m_ui.edit(gf::UIEditType::Simple, m_hostnameBuffer, gf::UIEditFilter::Default);

      m_ui.label("Name:");
      m_ui.edit(gf::UIEditType::Simple, m_nameBuffer, gf::UIEditFilter::Default);


      m_ui.layoutRow(gf::UILayout::Dynamic, 25.0f, gf::array(buttonRatios.begin(), buttonRatios.size()));

      if (m_ui.buttonLabel("Back")) {
        gf::Log::debug("Back\n");
        m_scenes.replaceScene(m_scenes.intro);
      }

      if (m_ui.buttonLabel("Connect")) {
        gf::Log::debug("Connect\n");
        m_scenes.replaceScene(m_scenes.waiting);
      }

    }

    m_ui.end();
  }

  void ConnectionScene::doRender(gf::RenderTarget& target) {
    renderWorldEntities(target);
    renderHudEntities(target);
    target.draw(m_ui);
  }

}