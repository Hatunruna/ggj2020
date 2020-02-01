#include "LobbyChat.h"

#include <gf/Color.h>
#include <gf/Log.h>

#include "common/Protocol.h"

namespace ggj {

  ImVec4 LobbyChat::toColor(gf::Id id) {
    gf::Color4f color;

    if (id == gf::InvalidId) { // Server message
      color = gf::Color::lighter(gf::Color::Red);
    }
    else {
      color = gf::Color::lighter(gf::Color::fromRgba32(static_cast<uint32_t>(id)));
    }

    return ImVec4(color.r, color.g, color.b, 1.0f);
  }

  LobbyChat::LobbyChat(ClientNetwork& network)
  : m_network(network)
  , m_autoscroll(false)
  {
    m_lineBuffer.clear();
  }

  void LobbyChat::appendMessage(const MessageData& message) {
    m_messages.push_back(message);
    m_autoscroll = true;
  }

  void LobbyChat::appendMessage(MessageData&& message) {
    m_messages.push_back(std::move(message));
    m_autoscroll = true;
  }

  void LobbyChat::display(int lines) {
    ImGui::Text("Chat");
    ImGui::Spacing();
    ImVec2 size(0.0f, lines * ImGui::GetTextLineHeightWithSpacing());

    if (ImGui::BeginChild("Messages", size, false)) {
      for (auto& message : m_messages) {
        std::string str = "[" + message.author + "] ";
        ImGui::TextColored(toColor(message.origin), str.c_str());
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

    //Fix InputText not resized correctly
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth());

    if (ImGui::InputText("###chat", m_lineBuffer.getData(), m_lineBuffer.getSize(), ImGuiInputTextFlags_EnterReturnsTrue) && m_lineBuffer[0] != '\0') {
      ClientChatMessage data;
      data.content = m_lineBuffer.getData();
      m_network.send(data);
      m_lineBuffer.clear();
      ImGui::SetKeyboardFocusHere(-1);
    }
  }

}
