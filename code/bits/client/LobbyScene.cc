#include "LobbyScene.h"

#include <cinttypes>

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Unused.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "common/Constants.h"
#include "common/ImGuiConstants.h"
#include "common/Protocol.h"

#include "Scenes.h"

namespace pem {

  LobbyScene::LobbyScene(Scenes& scenes, gf::ResourceManager& resources, ClientNetwork& network)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_network(network)
  , m_settings{ 0, 0, 0, 0 }
  , m_instance{ 0, 0 }
  , m_selectedRoom(-1)
  , m_chat(network)
  , m_backgorund(resources)
  {
    setClearColor(gf::Color::Black);

    m_roomBuffer[0] = '\0';
    m_nameBuffer[0] = '\0';

    addHudEntity(m_backgorund);
  }

  void LobbyScene::doProcessEvent(gf::Event& event) {
    ImGui_ImplGF_ProcessEvent(event);
  }

  void LobbyScene::doUpdate(gf::Time time) {
    if (!isActive()) {
      return;
    }

    ImGui_ImplGF_Update(time);

    gf::Packet packet;

    while (m_network.queue.poll(packet)) {
      switch (packet.getType()) {
        case ServerHello::type: {
          gf::Log::debug("(LOBBY) Receive ServerHello\n");
          auto data = packet.as<ServerHello>();
          m_settings = data.settings;
          m_instance.teams = m_settings.teamsMin;
          m_instance.playersByTeam = m_settings.playersByTeamMin;
          m_scenes.myPlayerId = data.playerId;
          break;
        }

        case ServerDisconnect::type:
          gf::Log::debug("(LOBBY) Receive ServerDisconnect\n");
          m_network.disconnect();
          m_scenes.replaceScene(*m_scenes.connection, m_scenes.fadeEffect, gf::seconds(0.4f));
//           m_scenes.replaceScene(m_scenes.connection);
          break;

        case ServerChangeName::type: {
          gf::Log::debug("(LOBBY) Receive ServerChangeName\n");
          auto data = packet.as<ServerChangeName>();
          std::strncpy(m_nameBuffer.data(), data.name.c_str(), 255);
          break;
        }

        case ServerListPlayers::type: {
          gf::Log::debug("(LOBBY) Receive ServerListPlayers\n");
          auto data = packet.as<ServerListPlayers>();
          m_players = std::move(data.players);

          m_playersView.clear();
          for (auto& player : m_players) {
            m_playersView.push_back(player.name.c_str());
          }
          break;
        }

        case ServerListRooms::type: {
          gf::Log::debug("(LOBBY) Receive ServerListRooms\n");
          auto data = packet.as<ServerListRooms>();
          m_rooms = std::move(data.rooms);

          int32_t index = 0;
          m_selectedRoom = -1;
          m_roomsView.clear();
          for (auto& room : m_rooms) {
            m_roomsView.push_back(room.name.c_str());
            if (room.id == m_selectedRoomData.id) {
              m_selectedRoom = index;
              m_selectedRoomData = m_rooms[m_selectedRoom];
            }
            ++index;
          }
          break;
        }

        case ServerJoinRoom::type: {
          gf::Log::debug("(LOBBY) Receive ServerJoinRoom\n");
          auto data = packet.as<ServerJoinRoom>();
          m_scenes.room->startRoom(data.settings);
          m_scenes.replaceScene(*m_scenes.room, m_scenes.fadeEffect, gf::seconds(0.4f));
//           m_scenes.replaceScene(m_scenes.room);
          // do not poll any more message as the next messages are for the room
          return;
        }

        case ServerChatMessage::type: {
          gf::Log::debug("(LOBBY) Receive ServerChatMessage\n");
          auto data = packet.as<ServerChatMessage>();
          m_chat.appendMessage(std::move(data.message));
          break;
        }

        case ServerError::type: {
          gf::Log::debug("(LOBBY) Receive ServerError\n");
          auto data = packet.as<ServerError>();
          MessageData message;
          message.origin = gf::InvalidId;
          message.author = "server";
          message.content = serverErrorString(data.reason);

          m_chat.appendMessage(std::move(message));
          break;
        }
      }
    }
  }

  void LobbyScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);
    auto position = coords.getCenter();

    // UI

    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(1000.0f, 0.0f));
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.5f));

    if (ImGui::Begin("Lobby", nullptr, DefaultWindowFlags)) {
      ImGui::Columns(3);

      ImGui::BeginGroup();
      ImGui::Text("Players");
      ImGui::Spacing();
      int selectedPlayer = -1;
      ImGui::PushItemWidth(-1.0f);
      ImGui::ListBox("##players", &selectedPlayer, m_playersView.data(), m_playersView.size(), 10);
      ImGui::EndGroup();

      ImGui::NextColumn();

      ImGui::BeginGroup();
      ImGui::Text("Join a room");
      ImGui::Spacing();
      ImGui::PushItemWidth(-1.0f);


      if (ImGui::ListBox("###rooms", &m_selectedRoom, m_roomsView.data(), m_roomsView.size(), 10)) {
        assert(m_selectedRoom != -1);
        m_selectedRoomData = m_rooms[m_selectedRoom];
      }

      ImGui::PopItemWidth();
      ImGui::Spacing();
      ImGui::Indent();

      if (ImGui::Button("Join", DefaultButtonSize) && m_selectedRoom != -1) {
        ClientJoinRoom data;
        data.room = m_rooms[m_selectedRoom].id;
        m_network.send(data);
      }

      ImGui::EndGroup();

      ImGui::NextColumn();

      ImGui::BeginGroup();
      m_chat.display(10);
      ImGui::EndGroup();

      ImGui::NextColumn();
      ImGui::Separator(); // second line

      ImGui::BeginGroup();
      ImGui::Text("Change name");
      ImGui::Spacing();
      ImGui::Text("Name:");
      ImGui::SameLine();
      ImGui::InputText("###player_name", m_nameBuffer.data(), m_nameBuffer.size());

      ImGui::Spacing();
      ImGui::Indent();

      if (ImGui::Button("Change name", DefaultButtonSize) && m_nameBuffer[0] != '\0') {
        ClientChangeName data;
        data.name = std::string(m_nameBuffer.data());
        m_network.send(data);
      }

      ImGui::EndGroup();

      ImGui::NextColumn();

      ImGui::BeginGroup();
      ImGui::Text("Room info");

      if (m_selectedRoom != -1) {
        ImGui::Indent();
        ImGui::Text("Name: %s", m_selectedRoomData.name.c_str());
        ImGui::Text("Players: %" PRIi32 "/%" PRIi32, m_selectedRoomData.players, m_selectedRoomData.settings.teams * m_selectedRoomData.settings.playersByTeam);
      }

      ImGui::EndGroup();

      ImGui::NextColumn();

      ImGui::BeginGroup();
      ImGui::Text("Create room");
      ImGui::Spacing();
      ImGui::Text("Name:");
      ImGui::SameLine();
      ImGui::InputText("###room_name", m_roomBuffer.data(), m_roomBuffer.size());
      ImGui::SliderScalar(" players", ImGuiDataType_S32, &m_instance.playersByTeam, &m_settings.playersByTeamMin, &m_settings.playersByTeamMax);

      ImGui::Spacing();
      ImGui::Indent();

      if (m_settings.teamsMin > 0 && ImGui::Button("Create room", DefaultButtonSize) && m_roomBuffer[0] != '\0') {
        ClientCreateRoom data;
        data.name = std::string(m_roomBuffer.data());
        data.settings = m_instance;
        m_network.send(data);
        m_roomBuffer[0] = '\0';
      }

      ImGui::EndGroup();

      ImGui::Columns();
      ImGui::Separator();

      ImGui::Spacing();
      ImGui::Indent();

      if (ImGui::Button("Back", DefaultButtonSize)) {
        m_scenes.replaceScene(*m_scenes.connection, m_scenes.fadeEffect, gf::seconds(0.4f));
//         m_scenes.replaceScene(m_scenes.connection);
        m_network.disconnect();
      }
    }

    ImGui::End();

    // Display

    renderWorldEntities(target, states);
    renderHudEntities(target, states);

    ImGui::Render();
    ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
  }

}
