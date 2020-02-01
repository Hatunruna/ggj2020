#include "GameChat.h"

#include <gf/Color.h>
#include <gf/Log.h>

#include <imgui.h>

#include "common/Protocol.h"

namespace ggj {

  GameChat::GameChat(ClientNetwork& network)
  : LobbyChat(network)
  , m_selectedUserName("")
  , m_selectedUserId(gf::InvalidId)
  {
  }

  void GameChat::display(int lines) {
    ImGui::Text("Chat");
    ImGui::Spacing();
    ImVec2 size(0.0f, lines * ImGui::GetTextLineHeightWithSpacing());

    if (ImGui::BeginChild("Messages", size, false)) {
      for (auto& message : m_messages) {
        std::string str = "[" + message.author + "] ";
        ImGui::TextColored(toColor(message.origin), str.c_str());

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && message.origin != gf::InvalidId)
        {
          m_selectedUserName = "@" + message.author;
          m_selectedUserId = message.origin;
          m_lineBuffer.clear();
        }

        ImGui::SameLine();

        if (message.origin == gf::InvalidId) {
          ImGui::PushStyleColor(ImGuiCol_Text, toColor(message.origin));
          ImGui::TextWrapped(message.content.c_str());
          ImGui::PopStyleColor();
        }
        else {
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

    size = ImVec2(0.0f, 1 * ImGui::GetTextLineHeightWithSpacing());

    if (ImGui::BeginChild("Input", size, false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
        //Detect if we are currently typing for a player
        if (m_selectedUserId != gf::InvalidId)
        {
            ImGui::Text(m_selectedUserName.c_str());
            ImGui::SameLine();
        }

        //Fix InputText not resized correctly
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth());

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
            gf::Log::debug("Backspace Hello!!!\n");
        }
    }

    ImGui::EndChild();
  }

}
