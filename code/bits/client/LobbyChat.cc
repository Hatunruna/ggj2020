#include "LobbyChat.h"

#include <gf/Color.h>
#include <gf/Log.h>

#include "common/Protocol.h"
#include "Singletons.h"

#define VOICE_IF(n) \
    if (content == n) \
    { \
      m_voice.setBuffer(gResourceManager().getSound("audio/taunts/" n ".ogg")); \
      m_voice.setVolume(FxsVolume); \
      m_voice.play(); \
    }

#define VOICE_ELSEIF(n) \
    if (content == n) \
    { \
      m_voice.setBuffer(gResourceManager().getSound("audio/taunts/" n ".ogg")); \
      m_voice.setVolume(FxsVolume); \
      m_voice.play(); \
    }



namespace pem {

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
    m_lineBuffer[0] = '\0';
  }

  void LobbyChat::appendMessage(const MessageData& message) {
    parseAndPlaySound(message.content);
    m_messages.push_back(message);
    m_autoscroll = true;
  }

  void LobbyChat::appendMessage(MessageData&& message) {
    parseAndPlaySound(message.content);
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

    if (ImGui::InputText("###chat", m_lineBuffer.data(), m_lineBuffer.size(), ImGuiInputTextFlags_EnterReturnsTrue) && m_lineBuffer[0] != '\0') {
      ClientChatMessage data;
      data.content = std::string(m_lineBuffer.data());
      m_network.send(data);
      m_lineBuffer[0] = '\0';
      ImGui::SetKeyboardFocusHere(-1);
    }
  }

  void LobbyChat::parseAndPlaySound(const std::string & content)
  {
    VOICE_IF("1")
    VOICE_ELSEIF("2")
    VOICE_ELSEIF("3")
    VOICE_ELSEIF("4")
    VOICE_ELSEIF("5")
    VOICE_ELSEIF("6")
    VOICE_ELSEIF("7")
    VOICE_ELSEIF("8")
    VOICE_ELSEIF("9")
    VOICE_ELSEIF("10")
    VOICE_ELSEIF("11")
    VOICE_ELSEIF("12")
    VOICE_ELSEIF("13")
    VOICE_ELSEIF("14")
    VOICE_ELSEIF("15")
    VOICE_ELSEIF("16")
    VOICE_ELSEIF("17")
    VOICE_ELSEIF("18")
    VOICE_ELSEIF("19")
    VOICE_ELSEIF("20")
    VOICE_ELSEIF("21")
    VOICE_ELSEIF("22")
    VOICE_ELSEIF("23")
    VOICE_ELSEIF("24")
    VOICE_ELSEIF("25")
    VOICE_ELSEIF("26")
    VOICE_ELSEIF("27")
    VOICE_ELSEIF("28")
    VOICE_ELSEIF("29")
    VOICE_ELSEIF("30")
    VOICE_ELSEIF("31")
    VOICE_ELSEIF("32")
    VOICE_ELSEIF("33")
    VOICE_ELSEIF("34")
    VOICE_ELSEIF("35")
    VOICE_ELSEIF("36")
    VOICE_ELSEIF("37")
    VOICE_ELSEIF("38")
    VOICE_ELSEIF("39")
    VOICE_ELSEIF("40")
    VOICE_ELSEIF("41")
    VOICE_ELSEIF("42")
  }

}
