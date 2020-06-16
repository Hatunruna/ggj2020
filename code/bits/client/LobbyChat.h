#ifndef PEM_LOBBY_CHAT_H
#define PEM_LOBBY_CHAT_H

#include <vector>

#include <imgui.h>

#include "common/ProtocolData.h"

#include "ClientNetwork.h"

#include <gf/ResourceManager.h>

#include <SFML/Audio.hpp>

namespace pem {

  class LobbyChat {
  public:
    static ImVec4 toColor(gf::Id id);

  public:
    LobbyChat(ClientNetwork& network);

    void appendMessage(const MessageData& message);
    void appendMessage(MessageData&& message);

    void display(int lines);

  protected:
    void parseAndPlaySound(const std::string & content);

  protected:
    ClientNetwork& m_network;
    std::array<char, 256> m_lineBuffer;
    std::vector<MessageData> m_messages;
    bool m_autoscroll;

    sf::Sound m_voice;
  };


}



#endif // PEM_LOBBY_CHAT_H
