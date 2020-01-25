#include "ConnectionScene.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Unused.h>

#include "Constants.h"
#include "Protocol.h"
#include "Scenes.h"

namespace ggj {

  ConnectionScene::ConnectionScene(Scenes& scenes, ClientNetwork& network, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_network(network)
  , m_ui(resources.getFont("DejaVuSans.ttf"), UICharacterSize)
  , m_hostnameBuffer(128)
  , m_nameBuffer(MaxPlayerNameLength + 1)
  , m_connectionAsked(false)
  {
    m_hostnameBuffer.append("localhost");
    m_nameBuffer.append("toto");
  }

  void ConnectionScene::doProcessEvent(gf::Event& event) {
    m_ui.processEvent(event);
  }

  void ConnectionScene::doUpdate(gf::Time time) {
    gf::unused(time);
  }

  void ConnectionScene::doRender(gf::RenderTarget& target) {
    gf::Coordinates coords(target);

    // UI

    if (m_ui.begin("Connect", gf::RectF::fromCenterSize(coords.getCenter(), { 400.0f, 180.0f }), gf::UIWindow::Border | gf::UIWindow::Title)) {

      if (m_network.isConnecting()) {
        m_ui.label("Connecting...", gf::UIAlignment::Center);

        if (m_network.isConnected()) {
          m_connectionAsked = false;
          m_scenes.replaceScene(m_scenes.lobby);

          ClientChangeName data;
          data.name = m_nameBuffer.asString();
          m_network.send(data);
        }
      } else {
        auto editRatios = { 0.25f, 0.75f };
        auto buttonRatios = { 0.5f, 0.5f };

        m_ui.layoutRow(gf::UILayout::Dynamic, UILineSize, gf::array(editRatios.begin(), editRatios.size()));

        m_ui.label("Hostname:");
        m_ui.edit(gf::UIEditType::Simple, m_hostnameBuffer, gf::UIEditFilter::Default);

        m_ui.label("Name:");
        m_ui.edit(gf::UIEditType::Simple, m_nameBuffer, gf::UIEditFilter::Default);


        m_ui.layoutRow(gf::UILayout::Dynamic, UILineSize, gf::array(buttonRatios.begin(), buttonRatios.size()));

        if (m_ui.buttonLabel("Back")) {
          m_connectionAsked = false;
          m_scenes.replaceScene(m_scenes.intro);
        }

        if (m_ui.buttonLabel("Connect")) {
          m_network.connect(m_hostnameBuffer.asString());
          m_connectionAsked = true;
        }

        if (m_connectionAsked) {
          m_ui.layoutRowDynamic(UILineSize, 1);
          m_ui.label("Error: unable to connect to server.", gf::UIAlignment::Center);
        }
      }

    }

    m_ui.end();

    // Display

    renderWorldEntities(target);
    renderHudEntities(target);
    target.draw(m_ui);
  }

}
