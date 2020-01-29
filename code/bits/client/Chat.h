#ifndef TMPGAME_CHAT_H
#define TMPGAME_CHAT_H

#include <vector>

#include <gf/StaticString.h>

#include "../common/ProtocolData.h"

#include "ClientNetwork.h"

namespace ggj {

  class Chat {
  public:
    Chat(ClientNetwork& network);

    void appendMessage(const MessageData& message);
    void appendMessage(MessageData&& message);

    void display(int lines);


  private:
    ClientNetwork& m_network;
    gf::StaticString<255> m_lineBuffer;
    std::vector<MessageData> m_messages;
    bool m_autoscroll;
  };


}



#endif // TMPGAME_CHAT_H
