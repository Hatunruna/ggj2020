#ifndef TMPGAME_PEM_PROTOCOL_H
#define TMPGAME_PEM_PROTOCOL_H

#include <gf/Id.h>
#include <gf/SerializationOps.h>

#include "Constants.h"
#include "GameSettings.h"
#include "PemTypes.h"
#include "ProtocolData.h"

using namespace gf::literals;

namespace ggj {

  /*
   * server -> client
   */

  struct PemServerInitRole {
    static constexpr gf::Id type = "PemServerInitRole"_id;
    CrewType role;
    CardType cards[MaxCards];
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerInitRole& data) {
    return ar | data.role | data.cards;
  }

}

#endif // TMPGAME_PEM_PROTOCOL_H
