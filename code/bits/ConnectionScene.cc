#include "ConnectionScene.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Unused.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "Constants.h"
#include "Protocol.h"
#include "Scenes.h"

namespace ggj {

  ConnectionScene::ConnectionScene(Scenes& scenes, ClientNetwork& network)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_network(network)
  , m_connectionAsked(false)
  {
    m_hostnameBuffer = "localhost";
    m_nameBuffer = "toto";
  }

  void ConnectionScene::doProcessEvent(gf::Event& event) {
    ImGui_ImplGF_ProcessEvent(event);
  }

  void ConnectionScene::doUpdate(gf::Time time) {
    ImGui_ImplGF_Update(time);
  }

  void ConnectionScene::doRender(gf::RenderTarget& target) {
    gf::Coordinates coords(target);

    auto windowPosition = coords.getCenter();
    auto buttonSize = ImVec2(170, 40);

    // UI
    ImGui::NewFrame();

//     ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::SetNextWindowPos(ImVec2(windowPosition.x, windowPosition.y), 0, ImVec2(0.5f, 0.5f));

    if (ImGui::Begin("Connect", nullptr, /* ImGuiWindowFlags_NoResize | */ ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)) {
      if (m_network.isConnecting()) {
        ImGui::Text("Connecting...");

        if (m_network.isConnected()) {
          m_connectionAsked = false;
          m_scenes.replaceScene(m_scenes.lobby);

          ClientChangeName data;
          data.name = m_nameBuffer.getData();
          m_network.send(data);
        }
      } else {
        ImGui::Text("Hostname:");
        ImGui::SameLine();
        float x = ImGui::GetCursorPosX();
        ImGui::InputText("###hostname", m_hostnameBuffer.getData(), m_hostnameBuffer.getSize());

        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::SetCursorPosX(x);
        ImGui::InputText("###name", m_nameBuffer.getData(), m_nameBuffer.getSize());

        ImGui::Indent();

        if (ImGui::Button("Back", buttonSize)) {
          m_connectionAsked = false;
          m_scenes.replaceScene(m_scenes.intro);
        }

        ImGui::SameLine();

        if (ImGui::Button("Connect", buttonSize)) {
          m_network.connect(m_hostnameBuffer.getData());
          m_connectionAsked = true;
        }

        if (m_connectionAsked) {
          ImGui::Text("Error: unable to connect to server.");
        }
      }

      ImGui::End();
    }

    // Display

    renderWorldEntities(target);
    renderHudEntities(target);

    ImGui::Render();
    ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
  }

}
