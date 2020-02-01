#include "GameChat.h"

#include <gf/Color.h>
#include <gf/Log.h>

#include <imgui.h>

#include "common/Protocol.h"
#include "common/ImGuiConstants.h"

namespace ggj {

  GameChat::GameChat(ClientNetwork& network, std::map<gf::Id, ClientPlayerData>& players)
  : LobbyChat(network)
  , m_selectedUserName("")
  , m_selectedUserId(gf::InvalidId)
  , m_players(players)
  {
  }

  void GameChat::display(int lines, gf::Coordinates& coordinates) {

    // Chat window
    gf::Vector2f chatWindowSize = coordinates.getRelativeSize({ 0.35f, 0.46f });
    gf::Vector2f chatWindowPos = coordinates.getRelativePoint({ 0.82f, 0.76f });

    ImGui::SetNextWindowSize(ImVec2(chatWindowSize[0], chatWindowSize[1]));
    ImGui::SetNextWindowPos(ImVec2(chatWindowPos[0], chatWindowPos[1]), 0, ImVec2(0.5f, 0.5f));

    if (ImGui::Begin("Chat", nullptr, DefaultWindowFlags))
    {
      ImGui::Columns(2);

      ImGui::BeginGroup();
      ImGui::Text("Messages");
      ImGui::Spacing();
      ImVec2 size(0.0f, chatWindowSize[1] - 4.5f * ImGui::GetTextLineHeightWithSpacing());

      if (ImGui::BeginChild("Messages", size, false)) {
        for (auto& message : m_messages) {
          std::string str = "[" + message.author + "] ";
          ImGui::TextColored(toColor(message.origin), str.c_str());

          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && message.origin != gf::InvalidId) {
            m_selectedUserName = "@" + message.author;
            m_selectedUserId = message.origin;
            m_lineBuffer.clear();
          }

          ImGui::SameLine();

          if (message.origin == gf::InvalidId) {
            ImGui::PushStyleColor(ImGuiCol_Text, toColor(message.origin));
            ImGui::TextWrapped(message.content.c_str());
            ImGui::PopStyleColor();
          } else if (message.recipient != gf::InvalidId) {
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

      size_t oldSize = strlen(m_lineBuffer.getData());

      if (ImGui::InputText("###chat", m_lineBuffer.getData(), m_lineBuffer.getSize(), ImGuiInputTextFlags_EnterReturnsTrue) && m_lineBuffer[0] != '\0') {
          ClientChatMessage data;
          data.content = m_lineBuffer.getData();

          if (m_selectedUserId != gf::InvalidId)
          {
            data.recipient = m_selectedUserId;
            m_selectedUserId = gf::InvalidId;
            m_selectedUserName = "";
          }

          m_network.send(data);
          m_lineBuffer.clear();
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

      size = ImVec2(0.0f, chatWindowSize[1] - 4.5f * ImGui::GetTextLineHeightWithSpacing());

      if (ImGui::BeginChild("Players", size, false)) {
        // List players
        for (auto &player: m_players) {
          ImGui::TextColored(toColor(player.second.id), player.second.name.c_str());
          
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            m_selectedUserName = "@" + player.second.name;
            m_selectedUserId = player.second.id;
            m_lineBuffer.clear();
          }
        }
      }
      ImGui::EndChild();
      ImGui::EndGroup();
    }
    ImGui::End();

  }

}
