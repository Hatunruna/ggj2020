#include "LobbyScene.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Unused.h>

#include "Constants.h"
#include "Protocol.h"
#include "Scenes.h"


namespace ggj {

  LobbyScene::LobbyScene(Scenes& scenes, ClientNetwork& network, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_network(network)
  , m_ui(resources.getFont("DejaVuSans.ttf"), 14)
  , m_roomBuffer(128)
  , m_lineBuffer(128)
  , m_nameBuffer(MaxPlayerNameLength)
  , m_selectedRoom(-1)
  {
  }

  void LobbyScene::doProcessEvent(gf::Event& event) {
    m_ui.processEvent(event);
  }

  void LobbyScene::doUpdate(gf::Time time) {
    gf::unused(time);

    ProtocolBytes bytes;

    while (m_network.queue.poll(bytes)) {
      switch (bytes.getType()) {
        case ServerDisconnect::type:
          m_network.disconnect();
          m_scenes.replaceScene(m_scenes.connection);
          break;

        case ServerListPlayers::type: {
          auto data = bytes.as<ServerListPlayers>();
          m_players = data.players; // TODO: move?
          break;
        }

        case ServerListRooms::type: {
          auto data = bytes.as<ServerListRooms>();
          m_rooms = data.rooms; // TODO: move?
          break;
        }

        case ServerJoinRoom::type: {
          m_scenes.replaceScene(m_scenes.room);
          // do not poll any more message as the next messages are for the room
          return;
        }

        case ServerChatMessage::type: {
          auto data = bytes.as<ServerChatMessage>();
          m_messages.push_back(std::move(data.message));
          break;
        }
      }
    }
  }

/*
 * - players list | rooms list + join | chat
 * - change name  | room info         | create room
 * - Back
 */
  void LobbyScene::doRender(gf::RenderTarget& target) {
    gf::Coordinates coords(target);

    // UI

    if (m_ui.begin("Connect", gf::RectF::fromCenterSize(coords.getCenter(), { 800.0f, 600.0f }), gf::UIWindow::Border | gf::UIWindow::Title)) {
      auto mainRatios = { 0.33f, 0.34f, 0.33f };

      /*
       * first line
       */

      m_ui.layoutRow(gf::UILayout::Dynamic, 300.0f, gf::array(mainRatios.begin(), mainRatios.size()));

      // players panel

      if (m_ui.groupBegin("Players", gf::UIWindow::Title | gf::UIWindow::Border)) {
//         m_ui.layoutRowDynamic(250.0f, 1);
//
//         if (m_ui.groupBegin("Player list", gf::UIWindow::Border)) {
          m_ui.layoutRowDynamic(UILineSize, 1);

          for (auto& player : m_players) {
            m_ui.label(player.name, gf::UIAlignment::Left);
          }

//           m_ui.groupEnd();
//         }

        m_ui.groupEnd();
      }

      // join room panel

      if (m_ui.groupBegin("Join a room", gf::UIWindow::Title | gf::UIWindow::Border)) {
        m_ui.layoutRowDynamic(215.0f, 1);

        if (m_ui.groupBegin("Room list", gf::UIWindow::Border)) {
          m_ui.layoutRowDynamic(UILineSize, 1);

          for (std::size_t i = 0; i < m_rooms.size(); ++i) {
            if (m_ui.selectableValueLabel(m_rooms[i].name, gf::UIAlignment::Left, (i == m_selectedRoom))) {
              m_selectedRoom = i;
            }
          }

          m_ui.groupEnd();
        }

        m_ui.layoutRowDynamic(UILineSize, 1);

        if (m_ui.buttonLabel("Join")) {
          ClientJoinRoom data;
          data.room = m_rooms[m_selectedRoom].id;
          m_network.send(data);
        }

        m_ui.groupEnd();
      }

      // chat

      if (m_ui.groupBegin("Chat", gf::UIWindow::Title | gf::UIWindow::Border)) {
        m_ui.layoutRowDynamic(215.0f, 1);

        if (m_ui.groupBegin("Room list", gf::UIWindow::Border)) {
          m_ui.layoutRowDynamic(180, 1);

          gf::UICharBuffer chat(128);

          for (auto& message : m_messages) {
            chat.append("[" + message.author + "] " + message.content + '\n');
          }

          m_ui.edit(gf::UIEditType::Box | gf::UIEdit::ReadOnly, chat);

          m_ui.groupEnd();
        }

        auto chatRatio = { 0.6f, 0.4f };
        m_ui.layoutRow(gf::UILayout::Dynamic, UILineSize, gf::array(chatRatio.begin(), chatRatio.size()));

        gf::UIEditEventFlags flags = m_ui.edit(gf::UIEditType::Field | gf::UIEdit::SigEnter, m_lineBuffer);

        if (m_ui.buttonLabel("Submit") || flags.test(gf::UIEditEvent::Commited)) {
          ClientChatMessage data;
          data.content = m_lineBuffer.asString();
          m_network.send(data);
          m_lineBuffer.clear();
        }

        m_ui.groupEnd();
      }


      // group info panel

      if (m_ui.groupBegin("Room info", gf::UIWindow::Title | gf::UIWindow::Border)) {

        m_ui.groupEnd();
      }

      /*
       * second line
       */

      m_ui.layoutRow(gf::UILayout::Dynamic, 70.0f, gf::array(mainRatios.begin(), mainRatios.size()));

      // create room panel

      if (m_ui.groupBegin("Create a room", gf::UIWindow::Title | gf::UIWindow::Border)) {
        auto editRatios = { 0.25f, 0.75f };
        m_ui.layoutRow(gf::UILayout::Dynamic, 25.0f, gf::array(editRatios.begin(), editRatios.size()));

        m_ui.label("Name:");
        m_ui.edit(gf::UIEditType::Simple, m_roomBuffer, gf::UIEditFilter::Default);

        m_ui.layoutRowDynamic(25.0f, 1);

        if (m_ui.buttonLabel("Create")) {
          ClientCreateRoom data;
          data.name = m_roomBuffer.asString();
          m_network.send(data);
        }

        m_ui.groupEnd();
      }

      /*
       * Third line
       */

      m_ui.layoutRowDynamic(25.0f, 1);

      if (m_ui.buttonLabel("Back")) {
        m_network.disconnect();
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
