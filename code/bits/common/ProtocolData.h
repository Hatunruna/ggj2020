#ifndef PEM_PROTOCOL_DATA_H
#define PEM_PROTOCOL_DATA_H

#include <string>

#include <gf/Id.h>

#include "GameSettings.h"

namespace pem {

  struct PlayerData {
    gf::Id id;
    std::string name;
    int32_t team = -1;
    bool ready = false;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PlayerData& data) {
    return ar | data.id | data.name | data.team | data.ready;
  }

  struct RoomData {
    gf::Id id;
    std::string name;
    GameInstanceSettings settings;
    int32_t players;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, RoomData& data) {
    return ar | data.id | data.name | data.settings | data.players;
  }

  struct MessageData {
    gf::Id origin;
    std::string author;
    std::string content;
    gf::Id recipient = gf::InvalidId;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, MessageData& data) {
    return ar | data.origin | data.author | data.content | data.recipient;
  }

}

#endif // PEM_PROTOCOL_DATA_H
