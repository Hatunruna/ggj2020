#include "ConnectionScene.h"

#include <cstring>

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

  ConnectionScene::ConnectionScene(Scenes& scenes, gf::ResourceManager& resources, ClientNetwork& network)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_network(network)
  , m_connectionAsked(false)
  , m_backgorund(resources)
  {
    setClearColor(gf::Color::Black);

    std::strncpy(m_hostnameBuffer.data(), "localhost", 256);
    std::strncpy(m_nameBuffer.data(), "toto", 256);

    addHudEntity(m_backgorund);
  }

  void ConnectionScene::doProcessEvent(gf::Event& event) {
    ImGui_ImplGF_ProcessEvent(event);
  }

  void ConnectionScene::doUpdate(gf::Time time) {
    ImGui_ImplGF_Update(time);
  }

  void ConnectionScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);
    auto position = coords.getCenter();

    // UI
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.5f));

    if (ImGui::Begin("Connect", nullptr, DefaultWindowFlags)) {
      if (m_network.isConnecting()) {
        ImGui::Text("Connecting...");

        if (m_network.isConnected()) {
          m_connectionAsked = false;
          m_scenes.replaceScene(m_scenes.lobby, m_scenes.fadeEffect, gf::seconds(0.4f));

          ClientHello data;
          data.name = std::string(m_nameBuffer.cbegin(), m_nameBuffer.cend());
          m_network.send(data);
        }
      } else {
        ImGui::Text("Hostname:");
        ImGui::SameLine();
        float x = ImGui::GetCursorPosX();
        ImGui::InputText("###hostname", m_hostnameBuffer.data(), m_hostnameBuffer.size());

        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::SetCursorPosX(x);
        ImGui::InputText("###name", m_nameBuffer.data(), m_nameBuffer.size());
        ImGui::SetItemDefaultFocus();

        ImGui::Indent();

        if (ImGui::Button("Back", DefaultButtonSize)) {
          m_connectionAsked = false;
          m_scenes.replaceScene(m_scenes.intro, m_scenes.fadeEffect, gf::seconds(0.4f));
        }

        ImGui::SameLine();

        if (ImGui::Button("Connect", DefaultButtonSize)) {
          m_network.connect(std::string(m_hostnameBuffer.cbegin(), m_hostnameBuffer.cend()));
          m_connectionAsked = true;
        }

        if (m_connectionAsked) {
          ImGui::Text("Error: unable to connect to server.");
        }
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
