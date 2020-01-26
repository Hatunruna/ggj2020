#include "RoomScene.h"

#include <cinttypes>

#include <gf/Coordinates.h>
#include <gf/Log.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "Constants.h"
#include "ImGuiConstants.h"
#include "Protocol.h"
#include "Scenes.h"

namespace ggj {

  namespace {

    struct TeamInfo {
      const char *name;
      gf::Color4f color;
    };

    constexpr TeamInfo g_teamInfo[] = {
      { "Team Red", gf::Color::Red },
      { "Team Blue", gf::Color::Blue },
      { "Team Green", gf::Color::Green },
      { "Team Yellow", gf::Color::Yellow },
      { "Team Magenta", gf::Color::Magenta },
      { "Team Cyan", gf::Color::Cyan },
    };

  }

  RoomScene::RoomScene(Scenes& scenes, ClientNetwork& network)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_network(network)
  {
  }

  void RoomScene::startRoom(const GameInstanceSettings& settings) {
    m_currentTeam = -1;
    m_settings = settings;
  }

  void RoomScene::doProcessEvent(gf::Event& event) {
    ImGui_ImplGF_ProcessEvent(event);
  }

  void RoomScene::doUpdate(gf::Time time) {
    ImGui_ImplGF_Update(time);

    ProtocolBytes bytes;

    while (m_network.queue.poll(bytes)) {
      switch (bytes.getType()) {
        case ServerLeaveRoom::type:
          m_currentTeam = -1;
          m_scenes.replaceScene(m_scenes.lobby);
          // do not poll any more message as the next messages are for the lobby
          return;

        case ServerChangeTeam::type: {
          auto data = bytes.as<ServerChangeTeam>();
          m_currentTeam = data.team;
          break;
        }

        case ServerListRoomPlayers::type: {
          auto data = bytes.as<ServerListRoomPlayers>();
          m_players = std::move(data.players);
          break;
        }

      }
    }
  }

  void RoomScene::doRender(gf::RenderTarget& target) {
    gf::Coordinates coords(target);
    auto position = coords.getCenter();

    // UI

    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(400.0f, 0.0f));

    if (ImGui::Begin("Room", nullptr, DefaultWindowFlags)) {

      auto ShowTeam = [&](const char *name, gf::Color4f color, int32_t team) {
        if (ImGui::Selectable(name, m_currentTeam == team)) {
          ClientChangeTeam data;
          data.team = team;
          m_network.send(data);
        }

        ImGui::Indent();

        auto lighter = gf::Color::lighter(color);

        for (auto& player : m_players) {
          if (player.team == team) {
            ImGui::TextColored(ImVec4(lighter.r, lighter.g, lighter.b, lighter.a), "%s", player.name.c_str());
          }
        }

        ImGui::Unindent();
      };

      ShowTeam("No team", gf::Color::White, -1);

      for (int32_t i = 0; i < m_settings.teams; ++i) {
        ShowTeam(g_teamInfo[i].name, g_teamInfo[i].color, i);
      }

      ImGui::Indent();
      ImGui::Spacing();

      if (ImGui::Button("Leave room", DefaultButtonSize)) {
        ClientLeaveRoom data;
        m_network.send(data);
      }

      ImGui::SameLine();

      if (ImGui::Button("Ready", DefaultButtonSize)) {
        // TODO
      }
    }

    ImGui::End();

    // Display

    renderWorldEntities(target);
    renderHudEntities(target);

    ImGui::Render();
    ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
  }

}
