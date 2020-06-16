#include "GameChat.h"

#include <gf/Color.h>
#include <gf/Log.h>

#include <imgui.h>

#include "common/Protocol.h"
#include "common/ImGuiConstants.h"

namespace pem {

  GameChat::GameChat(ClientNetwork& network, GameModel &model)
  : LobbyChat(network)
  , m_selectedUserName("")
  , m_selectedUserId(gf::InvalidId)
  , m_model(model)
  , m_spawnedChat(false)
  {
  }

  void GameChat::display(gf::Coordinates& coordinates) {

    ImGui::NewFrame();
    // Chat window
    if (!m_spawnedChat)
    {

      gf::Vector2f chatWindowSize = coordinates.getRelativeSize({ 0.35f, 0.46f });
      gf::Vector2f chatWindowPos = coordinates.getRelativePoint({ 0.82f, 0.76f });

      ImGui::SetNextWindowSize(ImVec2(chatWindowSize[0], chatWindowSize[1]));
      ImGui::SetNextWindowPos(ImVec2(chatWindowPos[0], chatWindowPos[1]), 0, ImVec2(0.5f, 0.5f));
      m_spawnedChat = true;
    }


    if (ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
      ImGui::Columns(2);

      ImGui::BeginGroup();
      ImGui::Text("Messages");
      ImGui::Spacing();
      ImVec2 size(0.0f, ImGui::GetWindowHeight() - 4.5f * ImGui::GetTextLineHeightWithSpacing());

      if (ImGui::BeginChild("Messages", size, false)) {
        for (auto& message : m_messages) {
          std::string str;
          if (message.recipient != gf::InvalidId)
          {
            str = "<" + message.author + "> ";
          }
          else
          {
            str = "[" + message.author + "] ";
          }

          ImGui::TextColored(toColor(message.origin), str.c_str());

          if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && message.origin != gf::InvalidId) {
            m_selectedUserName = "@" + message.author;
            m_selectedUserId = message.origin;
            m_lineBuffer[0] = '\0';
          }

          ImGui::SameLine();

          if (message.origin == gf::InvalidId) {
            ImGui::PushStyleColor(ImGuiCol_Text, toColor(message.origin));
            ImGui::TextWrapped(message.content.c_str());
            ImGui::PopStyleColor();
          } else {
            ImGui::TextWrapped(message.content.c_str());
          }
        }
      }

      if (m_autoscroll) {
        ImGui::SetScrollHereY(1.0f); // bottom
        m_autoscroll = false;
      }

      ImGui::EndChild();

      ImGui::Spacing();

      //Detect if we are currently typing for a player
      if (m_selectedUserId != gf::InvalidId)
      {
        ImGui::TextColored(toColor(m_selectedUserId), m_selectedUserName.c_str());
        ImGui::SameLine();
      }

      //Fix InputText not resized correctly
      ImGui::PushItemWidth(-1.0f);

      size_t oldSize = std::strlen(m_lineBuffer.data());

      if (ImGui::InputText("###chat", m_lineBuffer.data(), m_lineBuffer.size(), ImGuiInputTextFlags_EnterReturnsTrue) && m_lineBuffer[0] != '\0') {
          ClientChatMessage data;
          data.content = std::string(m_lineBuffer.cbegin(), m_lineBuffer.cend());

          if (m_selectedUserId != gf::InvalidId)
          {
            data.recipient = m_selectedUserId;
            m_selectedUserId = gf::InvalidId;
            m_selectedUserName = "";
          }

          m_network.send(data);
          m_lineBuffer[0] = '\0';
          ImGui::SetKeyboardFocusHere(-1);
      }

      if (ImGui::IsItemActive() && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Backspace)) && oldSize == 0)
      {
        m_selectedUserId = gf::InvalidId;
        m_selectedUserName = "";
      }

      ImGui::EndGroup();

      ImGui::NextColumn();

      ImGui::BeginGroup();
      ImGui::Text("Players");
      ImGui::Spacing();

      size = ImVec2(0.0f, ImGui::GetWindowHeight() - 4.5f * ImGui::GetTextLineHeightWithSpacing());

      if (ImGui::BeginChild("Players", size, false)) {
        // List players
        for (auto &player: m_model.players) {
          ImGui::TextColored(toColor(player.second.id), player.second.name.c_str());

          if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            m_selectedUserName = "@" + player.second.name;
            m_selectedUserId = player.second.id;
            m_lineBuffer[0] = '\0';
          }
        }
      }
      ImGui::EndChild();
      ImGui::EndGroup();
    }
    ImGui::End();

  }

}
