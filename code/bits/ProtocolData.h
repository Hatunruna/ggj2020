#ifndef TMPGAME_PROTOCOL_DATA_H
#define TMPGAME_PROTOCOL_DATA_H

#include <string>

#include <gf/Id.h>

namespace ggj {

  struct PlayerData {
    gf::Id id;
    std::string name;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PlayerData& data) {
    return ar | data.id | data.name;
  }

  struct RoomData {
    gf::Id id;
    std::string name;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, RoomData& data) {
    return ar | data.id | data.name;
  }

  struct MessageData {
    gf::Id origin;
    std::string author;
    std::string content;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, MessageData& data) {
    return ar | data.origin | data.author | data.content;
  }

}

#endif // TMPGAME_PROTOCOL_DATA_H
