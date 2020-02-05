#ifndef PEM_GAME_CHAT_H
#define PEM_GAME_CHAT_H

#include <vector>

#include <gf/StaticString.h>
#include <gf/Coordinates.h>

#include "common/ProtocolData.h"

#include "ClientNetwork.h"
#include "LobbyChat.h"
#include "ClientPlayerData.h"

namespace pem {

  class GameChat: public LobbyChat {
  public:
    GameChat(ClientNetwork& network, std::map<gf::Id, ClientPlayerData>& players);

    void display(gf::Coordinates& coordinates);

  private:
    std::string m_selectedUserName;
    gf::Id m_selectedUserId;

    std::map<gf::Id, ClientPlayerData>& m_players;

    bool m_spawnedChat;
  };

}



#endif // PEM_GAME_CHAT_H
