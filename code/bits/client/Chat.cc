#include "Chat.h"

#include <gf/Color.h>

#include <imgui.h>

#include "../common/Protocol.h"

namespace ggj {

  namespace {

    ImVec4 toColor(gf::Id id) {
      gf::Color4f color = gf::Color::lighter(gf::Color::fromRgba32(static_cast<uint32_t>(id)));
      return ImVec4(color.r, color.g, color.b, 1.0f);
    }

  }

  Chat::Chat(ClientNetwork& network)
  : m_network(network)
  , m_autoscroll(false)
  {
    m_lineBuffer.clear();
  }

  void Chat::appendMessage(const MessageData& message) {
    m_messages.push_back(message);
    m_autoscroll = true;
  }

  void Chat::appendMessage(MessageData&& message) {
    m_messages.push_back(std::move(message));
    m_autoscroll = true;
  }

  void Chat::display(int lines) {
    ImGui::Text("Chat");
    ImGui::Spacing();
    ImVec2 size(0.0f, lines * ImGui::GetTextLineHeightWithSpacing());

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
  }

}
