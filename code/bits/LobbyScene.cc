#include "LobbyScene.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>

#include "Constants.h"
#include "Scenes.h"

namespace ggj {

  LobbyScene::LobbyScene(Scenes& scenes, ClientNetwork& network, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_network(network)
  , m_ui(resources.getFont("DejaVuSans.ttf"))
  , m_roomBuffer(128)
  , m_selectedRoom(-1)
  {
  }

  void LobbyScene::doProcessEvent(gf::Event& event) {
    m_ui.processEvent(event);
  }

  void LobbyScene::doUpdate(gf::Time time) {
    ServerPacket packet;

    while (m_network.queue.poll(packet)) {
      // TODO
      switch (packet.type) {

      }
    }
  }

  void LobbyScene::doRender(gf::RenderTarget& target) {
    gf::Coordinates coords(target);

    // UI

    if (m_ui.begin("Connect", gf::RectF::fromCenterSize(coords.getCenter(), { 600.0f, 400.0f }), gf::UIWindow::Border | gf::UIWindow::Title)) {
      auto mainRatios = { 0.5f, 0.5f };
      m_ui.layoutRow(gf::UILayout::Dynamic, 300.0f, gf::array(mainRatios.begin(), mainRatios.size()));

      // create room panel

      if (m_ui.groupBegin("Create a room", gf::UIWindow::Title | gf::UIWindow::Border)) {
        auto editRatios = { 0.25f, 0.75f };
        m_ui.layoutRow(gf::UILayout::Dynamic, 25.0f, gf::array(editRatios.begin(), editRatios.size()));

        m_ui.label("Name:");
        m_ui.edit(gf::UIEditType::Simple, m_roomBuffer, gf::UIEditFilter::Default);

        m_ui.layoutRowDynamic(25.0f, 1);

        if (m_ui.buttonLabel("Create")) {

        }

        m_ui.groupEnd();
      }

      // join room panel

      if (m_ui.groupBegin("Join a room", gf::UIWindow::Title | gf::UIWindow::Border)) {
        m_ui.layoutRowDynamic(200.0f, 1);

        if (m_ui.groupBegin("Room list", gf::UIWindow::Border)) {
          m_ui.layoutRowDynamic(25.0f, 1);

          for (std::size_t i = 0; i < m_rooms.size(); ++i) {
            if (m_ui.selectableValueLabel(m_rooms[i].data(), gf::UIAlignment::Left, (i == m_selectedRoom))) {
              m_selectedRoom = i;
            }
          }

          m_ui.groupEnd();
        }

        m_ui.layoutRowDynamic(25.0f, 1);

        if (m_ui.buttonLabel("Join")) {
          m_scenes.replaceScene(m_scenes.room);
        }

        m_ui.groupEnd();
      }


      m_ui.layoutRowDynamic(25.0f, 1);

      if (m_ui.buttonLabel("Back")) {
        m_scenes.replaceScene(m_scenes.connection);
      }

    }

    m_ui.end();

    // Display

    renderWorldEntities(target);
    renderHudEntities(target);
    target.draw(m_ui);
  }

}
