#ifndef TMPGAME_GAME_CHAT_H
#define TMPGAME_GAME_CHAT_H

#include <vector>

#include <gf/StaticString.h>
#include <gf/Coordinates.h>

#include "common/ProtocolData.h"

#include "ClientNetwork.h"
#include "LobbyChat.h"
#include "ClientPlayerData.h"

namespace ggj {

  class GameChat: public LobbyChat {
  public:
    GameChat(ClientNetwork& network, std::map<gf::Id, ClientPlayerData>& players);

    void display(int lines, gf::Coordinates& coordinates);

  private:
    std::string m_selectedUserName;
    gf::Id m_selectedUserId;

    std::map<gf::Id, ClientPlayerData>& m_players;
  };

}



#endif // TMPGAME_GAME_CHAT_H
