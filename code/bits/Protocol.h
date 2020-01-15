#ifndef TMPGAME_PROTOCOL_H
#define TMPGAME_PROTOCOL_H

#include <array>

#include <gf/Id.h>
#include <gf/SerializationOps.h>

#include "Constants.h"

namespace ggj {

  /*
   * server -> client
   */

  enum class ServerPacketType : uint16_t {
    PlayerId,
  };

  struct PlayerId {
    gf::Id id;
  };

  struct ServerPacket {
    ServerPacketType type;

    union {
      PlayerId playerId;
    };

  };

  template<typename Archive>
  Archive& operator|(Archive& ar, ServerPacket& packet) {
    ar | packet.type;

    switch (packet.type) {
      case ServerPacketType::PlayerId:
        ar | packet.playerId.id;
        break;
    }

    return ar;
  }


  /*
   * client -> server
   */

  enum class ClientPacketType : uint16_t {
    PlayerName,
  };

  struct PlayerName {
    std::array<char, MaxPlayerNameLength + 1> name;
  };

  struct ClientPacket {
    ClientPacketType type;

    union {
      PlayerName playerName;
    };
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, ClientPacket& packet) {
    ar | packet.type;

    switch (packet.type) {
      case ClientPacketType::PlayerName:
        ar | packet.playerName.name;
        break;
    }

    return ar;
  }

}

#endif // TMPGAME_PROTOCOL_H
