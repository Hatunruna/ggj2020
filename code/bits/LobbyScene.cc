#include "LobbyScene.h"

#include <cinttypes>

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Unused.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "Constants.h"
#include "ImGuiConstants.h"
#include "Protocol.h"
#include "Scenes.h"

namespace ggj {

  namespace {

    ImVec4 toColor(gf::Id id) {
      gf::Color4f color = gf::Color::lighter(gf::Color::fromRgba32(static_cast<uint32_t>(id)));
      return ImVec4(color.r, color.g, color.b, 1.0f);
    }

  }

  LobbyScene::LobbyScene(Scenes& scenes, ClientNetwork& network)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_network(network)
  , m_settings{ 0, 0, 0, 0 }
  , m_instance{ 0, 0 }
  , m_selectedRoom(-1)
  , m_autoscroll(false)
  {
    m_roomBuffer.clear();
    m_lineBuffer.clear();
    m_nameBuffer.clear();
  }

  void LobbyScene::doProcessEvent(gf::Event& event) {
    ImGui_ImplGF_ProcessEvent(event);
  }

  void LobbyScene::doUpdate(gf::Time time) {
    ImGui_ImplGF_Update(time);

    ProtocolBytes bytes;

    while (m_network.queue.poll(bytes)) {
      switch (bytes.getType()) {
        case ServerHello::type: {
          auto data = bytes.as<ServerHello>();
          m_settings = data.settings;
          m_instance.teams = m_settings.teamsMin;
          m_instance.playersByTeam = m_settings.playersByTeamMin;
          break;
        }

        case ServerDisconnect::type:
          m_network.disconnect();
          m_scenes.replaceScene(m_scenes.connection);
          break;

        case ServerChangeName::type: {
          auto data = bytes.as<ServerChangeName>();
          m_nameBuffer = data.name;
          break;
        }

        case ServerListPlayers::type: {
          auto data = bytes.as<ServerListPlayers>();
          m_players = std::move(data.players);

          m_playersView.clear();
          for (auto& player : m_players) {
            m_playersView.push_back(player.name.c_str());
          }
          break;
        }

        case ServerListRooms::type: {
          auto data = bytes.as<ServerListRooms>();
          m_rooms = std::move(data.rooms);

          m_roomsView.clear();
          for (auto& room : m_rooms) {
            m_roomsView.push_back(room.name.c_str());
          }
          break;
        }

        case ServerAnswerRoom::type: {
          auto data = bytes.as<ServerAnswerRoom>();
          m_selectedRoomName = std::move(data.name);
          m_selectedRoomSettings = data.settings;
          break;
        }

        case ServerJoinRoom::type: {
          m_scenes.replaceScene(m_scenes.room);
          // do not poll any more message as the next messages are for the room
          return;
        }

        case ServerChatMessage::type: {
          auto data = bytes.as<ServerChatMessage>();
          m_autoscroll = true;
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
        ClientQueryRoom data;
        data.room = m_rooms[m_selectedRoom].id;
        m_network.send(data);
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
      ImGui::Text("Chat");
      ImGui::Spacing();
      ImVec2 size(0.0f, 10 * ImGui::GetTextLineHeightWithSpacing());

      if (ImGui::BeginChild("Messages", size, false)) {
        for (auto& message : m_messages) {
          std::string str = "[" + message.author + "] ";
          ImGui::TextColored(toColor(message.origin), str.c_str());
          ImGui::SameLine();
          ImGui::TextWrapped(message.content.c_str());
        }
      }

      if (m_autoscroll) {
        ImGui::SetScrollHereY(1.0f); // bottom
        m_autoscroll = false;
      }

      ImGui::EndChild();

      ImGui::Spacing();

      if (ImGui::InputText("###chat", m_lineBuffer.getData(), m_lineBuffer.getSize(), ImGuiInputTextFlags_EnterReturnsTrue) && m_lineBuffer[0] != '\0') {
        ClientChatMessage data;
        data.content = m_lineBuffer.getData();
        m_network.send(data);
        m_lineBuffer.clear();
        ImGui::SetKeyboardFocusHere(-1);
      }

      ImGui::EndGroup();

      ImGui::NextColumn();
      ImGui::Separator(); // second line

      ImGui::BeginGroup();
      ImGui::Text("Change name");
      ImGui::Spacing();
      ImGui::Text("Name:");
      ImGui::SameLine();
      ImGui::InputText("###player_name", m_nameBuffer.getData(), m_nameBuffer.getSize());

      ImGui::Spacing();
      ImGui::Indent();

      if (ImGui::Button("Change name", DefaultButtonSize) && m_nameBuffer[0] != '\0') {
        ClientChangeName data;
        data.name = m_nameBuffer.getData();
        m_network.send(data);
      }

      ImGui::EndGroup();

      ImGui::NextColumn();

      ImGui::BeginGroup();
      ImGui::Text("Room info");

      if (!m_selectedRoomName.empty()) {
        ImGui::Indent();
        ImGui::Text("Name: %s", m_selectedRoomName.c_str());
        ImGui::Text("Teams: %" PRIi32, m_selectedRoomSettings.teams);
        ImGui::Text("Players/team: %" PRIi32, m_selectedRoomSettings.playersByTeam);
      }

      ImGui::EndGroup();

      ImGui::NextColumn();

      ImGui::BeginGroup();
      ImGui::Text("Create room");
      ImGui::Spacing();
      ImGui::Text("Name:");
      ImGui::SameLine();
      ImGui::InputText("###room_name", m_roomBuffer.getData(), m_roomBuffer.getSize());

      if (m_settings.teamsMin == m_settings.teamsMax) {
        ImGui::Text("Teams:");
        ImGui::SameLine();
        ImGui::Text("%" PRIi32, m_instance.teams);
      } else {
        ImGui::SliderScalar("teams", ImGuiDataType_S32, &m_instance.teams, &m_settings.teamsMin, &m_settings.teamsMax);
      }

      if (m_settings.playersByTeamMin == m_settings.playersByTeamMax) {
        ImGui::Text("Players/team:");
        ImGui::SameLine();
        ImGui::Text("%" PRIi32, m_instance.playersByTeam);
      } else {
        ImGui::SliderScalar("#/team", ImGuiDataType_S32, &m_instance.playersByTeam, &m_settings.playersByTeamMin, &m_settings.playersByTeamMax);
      }

      ImGui::Spacing();
      ImGui::Indent();

      if (m_settings.teamsMin > 0 && ImGui::Button("Create room", DefaultButtonSize) && m_roomBuffer[0] != '\0') {
        ClientCreateRoom data;
        data.name = m_roomBuffer.getData();
        data.settings = m_instance;
        m_network.send(data);
        m_roomBuffer.clear();
      }

      ImGui::EndGroup();

      // TODO: back button?
    }

    ImGui::End();

    // Display

    renderWorldEntities(target);
    renderHudEntities(target);

    ImGui::Render();
    ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
  }

}
