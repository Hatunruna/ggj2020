#ifndef TMPGAME_GAME_CHAT_H
#define TMPGAME_GAME_CHAT_H

#include <vector>

#include <gf/StaticString.h>
#include <gf/Coordinates.h>

#include "common/ProtocolData.h"

#include "ClientNetwork.h"
#include "LobbyChat.h"

namespace ggj {

  class GameChat: public LobbyChat {
  public:
    GameChat(ClientNetwork& network, std::vector<PlayerData>& players);

    void display(int lines, gf::Coordinates& coordinates);

  private:
    std::string m_selectedUserName;
    gf::Id m_selectedUserId;

    std::vector<PlayerData>& m_players;
  };

}



#endif // TMPGAME_GAME_CHAT_H
