#ifndef PEM_GAME_CHAT_H
#define PEM_GAME_CHAT_H

#include <vector>

#include <gf/StaticString.h>
#include <gf/Coordinates.h>

#include "common/ProtocolData.h"

#include "ClientNetwork.h"
#include "GameModel.h"
#include "LobbyChat.h"

namespace pem {

  class GameChat: public LobbyChat {
  public:
    GameChat(ClientNetwork& network, GameModel &model);

    void display(gf::Coordinates& coordinates);

  private:
    std::string m_selectedUserName;
    gf::Id m_selectedUserId;

    GameModel &m_model;

    bool m_spawnedChat;
  };

}



#endif // PEM_GAME_CHAT_H
