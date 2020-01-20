#ifndef TMPGAME_PROTOCOL_H
#define TMPGAME_PROTOCOL_H

#include <array>

#include <gf/Id.h>
#include <gf/SerializationOps.h>
#include <gf/StaticString.h>

#include "Constants.h"

namespace ggj {

  using PlayerName = gf::StaticString<MaxPlayerNameLength>;
  using RoomName = gf::StaticString<MaxRoomNameLength>;

  /*
   * server -> client
   */

  enum class ServerPacketType : uint16_t {
    Disconnect,
    PlayerId,
    PlayerOther,
  };

  struct PlayerId {
    gf::Id id;
  };

  struct PlayerOther {
    gf::Id id;
    PlayerName name;
  };

  struct ServerPacket {
    ServerPacketType type;

    union {
      PlayerId playerId;
      PlayerOther playerOther;
    };

  };

  template<typename Archive>
  Archive& operator|(Archive& ar, ServerPacket& packet) {
    ar | packet.type;

    switch (packet.type) {
      case ServerPacketType::Disconnect:
        // no data
        break;
      case ServerPacketType::PlayerId:
        ar | packet.playerId.id;
        break;

      case ServerPacketType::PlayerOther:
        ar | packet.playerOther.id | packet.playerOther.name;
        break;
    }

    return ar;
  }


  /*
   * client -> server
   */

  enum class ClientPacketType : uint16_t {
    Disconnect,
    PlayerIdentity,
  };

  struct PlayerIdentity {
    PlayerName name;
  };

  struct ClientPacket {
    ClientPacketType type;

    union {
      PlayerIdentity playerIdentity;
    };
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, ClientPacket& packet) {
    ar | packet.type;

    switch (packet.type) {
      case ClientPacketType::Disconnect:
        // no data
        break;
      case ClientPacketType::PlayerIdentity:
        ar | packet.playerIdentity.name;
        break;
    }

    return ar;
  }

}

#endif // TMPGAME_PROTOCOL_H
