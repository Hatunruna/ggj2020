#include "RoomScene.h"

#include <cinttypes>
#include <algorithm>

#include <gf/Coordinates.h>
#include <gf/Log.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "common/Constants.h"
#include "common/ImGuiConstants.h"
#include "common/Protocol.h"

#include "Scenes.h"

namespace pem {

  RoomScene::RoomScene(Scenes& scenes, gf::ResourceManager& resources, ClientNetwork& network)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_network(network)
  , m_chat(network)
  , m_backgorund(resources)
  {
    addHudEntity(m_backgorund);
  }

  void RoomScene::startRoom(const GameInstanceSettings& settings) {
    m_currentTeam = -1;
    m_settings = settings;
    m_ready = false;

    ClientChangeTeam data;
    data.team = 0;
    m_network.send(data);
  }

  void RoomScene::doProcessEvent(gf::Event& event) {
    ImGui_ImplGF_ProcessEvent(event);
  }

  void RoomScene::doUpdate(gf::Time time) {
    if (!isActive()) {
      return;
    }

    ImGui_ImplGF_Update(time);

    ProtocolBytes bytes;

    while (m_network.queue.poll(bytes)) {
      switch (bytes.getType()) {
        case ServerLeaveRoom::type:
          gf::Log::debug("(ROOM) Receive ServerLeaveRoom\n");
          m_currentTeam = -1;
          m_scenes.replaceScene(m_scenes.lobby, m_scenes.fadeEffect, gf::seconds(0.4f));
//           m_scenes.replaceScene(m_scenes.lobby);
          // do not poll any more message as the next messages are for the lobby
          return;

        case ServerChangeTeam::type: {
          gf::Log::debug("(ROOM) Receive ServerChangeTeam\n");
          auto data = bytes.as<ServerChangeTeam>();
          m_currentTeam = data.team;
          break;
        }

        case ServerReady::type: {
          gf::Log::debug("(ROOM) Receive ServerReady\n");
          auto data = bytes.as<ServerReady>();
          m_ready = data.ready;
          break;
        }

        case ServerListRoomPlayers::type: {
          gf::Log::debug("(ROOM) Receive ServerListRoomPlayers\n");
          auto data = bytes.as<ServerListRoomPlayers>();
          m_players = std::move(data.players);
          break;
        }

        case ServerChatMessage::type: {
          gf::Log::debug("(ROOM) Receive ServerChatMessage\n");
          auto data = bytes.as<ServerChatMessage>();
          m_chat.appendMessage(std::move(data.message));
          break;
        }

        case ServerStartGame::type: {
          gf::Log::debug("(ROOM) Receive ServerStartGame\n");
          m_scenes.replaceScene(m_scenes.game, m_scenes.glitchEffect, gf::seconds(0.4f));
//           m_scenes.replaceScene(m_scenes.game);
          m_scenes.game.initialize(m_players);
          // do not poll any more message as the next messages are for the game
          return;
        }

        case ServerError::type: {
          gf::Log::debug("(ROOM) Receive ServerError\n");
          auto data = bytes.as<ServerError>();
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

  void RoomScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);
    auto position = coords.getCenter();

    // UI

    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(1000.0f, 0.0f));

    if (ImGui::Begin("Room", nullptr, DefaultWindowFlags)) {
      ImGui::Text("Players required: %" PRIi32, m_settings.playersByTeam);
      ImGui::Separator();

      ImGui::Columns(2);

      ImGui::BeginGroup();
      ImGui::Text("Players");
      ImGui::Spacing();

      for (auto& player : m_players) {
        if (player.team == -1)
        {
          ImGui::BulletText("%s [connecting]", player.name.c_str());
        }
        else
        {
          ImGui::BulletText("%s [%s]", player.name.c_str(), player.ready ? "ready" : "not ready");
        }
      }

      for (int i = m_players.size(); i < m_settings.playersByTeam; ++i) {
        ImGui::BulletText("-");
      }

      ImGui::EndGroup();

      ImGui::NextColumn();

      m_chat.display(10);

      ImGui::Columns();

      ImGui::Spacing();

      if (!m_ready) {
        ImGui::Indent();

        if (ImGui::Button("Leave room", DefaultButtonSize)) {
          ClientLeaveRoom data;
          m_network.send(data);
        }

        ImGui::SameLine();

        if (ImGui::Button("Ready", DefaultButtonSize)) {
          ClientReady data;
          data.ready = true;
          m_network.send(data);
        }
      } else {
        int32_t readyPlayers = std::count_if(m_players.begin(), m_players.end(), [](auto& player) { return player.ready; });
//         ImGui::Text("Waiting for other players...");
        ImGui::PushItemWidth(-1.0f);
        ImGui::ProgressBar(static_cast<float>(readyPlayers) / (m_settings.teams * m_settings.playersByTeam), DefaultProgressSize, "Waiting players...");
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
