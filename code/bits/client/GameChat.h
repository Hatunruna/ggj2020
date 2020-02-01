#ifndef TMPGAME_GAME_CHAT_H
#define TMPGAME_GAME_CHAT_H

#include <vector>

#include <gf/StaticString.h>

#include "common/ProtocolData.h"

#include "ClientNetwork.h"
#include "LobbyChat.h"

namespace ggj {

  class GameChat: public LobbyChat {
  public:
    GameChat(ClientNetwork& network);

    virtual void display(int lines) override;

  private:
    std::string m_selectedUserName;
    gf::Id m_selectedUserId;
  };

}



#endif // TMPGAME_GAME_CHAT_H
