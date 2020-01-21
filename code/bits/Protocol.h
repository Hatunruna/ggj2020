#ifndef TMPGAME_PROTOCOL_H
#define TMPGAME_PROTOCOL_H

#include <array>

#include <gf/Id.h>
#include <gf/SerializationOps.h>
#include <gf/StaticString.h>

#include "Constants.h"

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

  struct ClientCreateRoom {
    static constexpr gf::Id type = "ClientCreateRoom"_id;
    std::string name;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, ClientCreateRoom& data) {
    return ar | data.name;
  }

}

#endif // TMPGAME_PROTOCOL_H
