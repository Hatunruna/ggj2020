#ifndef TMPGAME_LOBBY_CHAT_H
#define TMPGAME_LOBBY_CHAT_H

#include <vector>

#include <gf/StaticString.h>

#include <imgui.h>

#include "common/ProtocolData.h"

#include "ClientNetwork.h"

namespace ggj {

  class LobbyChat {
  public:
    static ImVec4 toColor(gf::Id id);

  public:
    LobbyChat(ClientNetwork& network);

    void appendMessage(const MessageData& message);
    void appendMessage(MessageData&& message);

    virtual void display(int lines);


  protected:
    ClientNetwork& m_network;
    gf::StaticString<255> m_lineBuffer;
    std::vector<MessageData> m_messages;
    bool m_autoscroll;
  };


}



#endif // TMPGAME_LOBBY_CHAT_H
