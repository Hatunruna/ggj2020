#ifndef TMPGAME_PROTOCOL_H
#define TMPGAME_PROTOCOL_H

#include <string>
#include <vector>

#include <gf/Id.h>
#include <gf/SerializationOps.h>

#include "Constants.h"
#include "ProtocolData.h"

using namespace gf::literals;

namespace ggj {

  /*
   * server -> client
   */

  struct ServerDisconnect {
    static constexpr gf::Id type = "ServerDisconnect"_id;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, ServerDisconnect&) {
    return ar;
  }

  struct ServerJoinRoom {
    static constexpr gf::Id type = "ServerJoinRoom"_id;
    gf::Id room;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, ServerJoinRoom& data) {
    return ar | data.room;
  }

  struct ServerChatMessage {
    static constexpr gf::Id type = "ServerChatMessage"_id;
    MessageData message;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, ServerChatMessage& data) {
    return ar | data.message;
  }

  struct ServerListPlayers {
    static constexpr gf::Id type = "ServerListPlayers"_id;
    std::vector<PlayerData> players;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, ServerListPlayers& data) {
    return ar | data.players;
  }

  struct ServerListRooms {
    static constexpr gf::Id type = "ServerListRooms"_id;
    std::vector<RoomData> rooms;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, ServerListRooms& data) {
    return ar | data.rooms;
  }


  /*
   * client -> server
   */

  struct ClientDisconnect {
    static constexpr gf::Id type = "ClientDisconnect"_id;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, ClientDisconnect&) {
    return ar;
  }

  struct ClientChangeName {
    static constexpr gf::Id type = "ClientChangeName"_id;
    std::string name;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, ClientChangeName& data) {
    return ar | data.name;
  }

  struct ClientCreateRoom {
    static constexpr gf::Id type = "ClientCreateRoom"_id;
    std::string name;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, ClientCreateRoom& data) {
    return ar | data.name;
  }

  struct ClientJoinRoom {
    static constexpr gf::Id type = "ClientJoinRoom"_id;
    gf::Id room;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, ClientJoinRoom& data) {
    return ar | data.room;
  }

  struct ClientChatMessage {
    static constexpr gf::Id type = "ClientChatMessage"_id;
    std::string content;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, ClientChatMessage& data) {
    return ar | data.content;
  }

}

#endif // TMPGAME_PROTOCOL_H
