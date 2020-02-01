#ifndef TMPGAME_PEM_PROTOCOL_H
#define TMPGAME_PEM_PROTOCOL_H

#include <array>

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
    std::array<CardType, MaxCards> cards;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerInitRole& data) {
    return ar | data.role | data.cards;
  }

  struct PemServerStartVoteForCaptain {
    static constexpr gf::Id type = "PemServerStartVoteForCaptain"_id;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerStartVoteForCaptain&) {
    return ar;
  }

  struct PemServerChooseCaptain {
    static constexpr gf::Id type = "PemServerChooseCaptain"_id;
    gf::Id member;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerChooseCaptain& data) {
    return ar | data.member;
  }

  struct PemServerStartMoveAndPlay {
    static constexpr gf::Id type = "PemServerStartMoveAndPlay"_id;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerStartMoveAndPlay&) {
    return ar;
  }

  struct PemServerUpdateShip {
    static constexpr gf::Id type = "PemServerUpdateShip"_id;
    std::map<PlaceType, bool> state; // true = seemingly working
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerUpdateShip& data) {
    return ar | data.state;
  }

  struct PemServerResolution {
    static constexpr gf::Id type = "PemServerResolution"_id;

  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerResolution& data) {
    return ar;
  }


  /*
   * client -> server
   */

  struct PemClientVoteForCaptain {
    static constexpr gf::Id type = "PemClientVoteForCaptain"_id;
    gf::Id member;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemClientVoteForCaptain& data) {
    return ar | data.member;
  }

  struct PemClientStartMoveAndPlay {
    static constexpr gf::Id type = "PemClientStartMoveAndPlay"_id;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemClientStartMoveAndPlay&) {
    return ar;
  }

  struct PemClientMoveAndPlay {
    static constexpr gf::Id type = "PemClientMoveAndPlay"_id;
    PlaceType place;
    CardType card;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemClientMoveAndPlay& data) {
    return ar | data.place | data.card;
  }


}

#endif // TMPGAME_PEM_PROTOCOL_H
