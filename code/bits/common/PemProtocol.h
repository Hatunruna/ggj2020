#ifndef PEM_PEM_PROTOCOL_H
#define PEM_PEM_PROTOCOL_H

#include <array>

#include <gf/Id.h>
#include <gf/SerializationOps.h>

#include "Constants.h"
#include "GameSettings.h"
#include "PemTypes.h"
#include "ProtocolData.h"

using namespace gf::literals;

namespace pem {

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

  struct PemServerStartMoveAndPlay {
    static constexpr gf::Id type = "PemServerStartMoveAndPlay"_id;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerStartMoveAndPlay&) {
    return ar;
  }

  struct PemServerUpdateShip {
    static constexpr gf::Id type = "PemServerUpdateShip"_id;
    std::map<PlaceType, bool> states; // true = seemingly working
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerUpdateShip& data) {
    return ar | data.states;
  }

  // struct PemServerResolution {
  //   static constexpr gf::Id type = "PemServerResolution"_id;
  //   std::vector<Resolution> conclusion;
  // };

  // template<typename Archive>
  // Archive operator|(Archive& ar, PemServerResolution& data) {
  //   return ar | data.conclusion;
  // }

  struct PemServerStartVoteForPrisoner {
    static constexpr gf::Id type = "PemServerStartVoteForPrisoner"_id;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerStartVoteForPrisoner&) {
    return ar;
  }

  struct PemServerChoosePrisoner {
    static constexpr gf::Id type = "PemServerChoosePrisoner"_id;
    gf::Id member;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerChoosePrisoner& data) {
    return ar | data.member;
  }

  struct PemServerReleasePrisoner {
    static constexpr gf::Id type = "PemServerReleasePrisoner"_id;
    gf::Id prisoner;
    gf::Id deliverer; // != InvalidId when a release card has been played
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerReleasePrisoner& data) {
    return ar | data.prisoner | data.deliverer;
  }

  struct PemServerMissionStatus {
    static constexpr gf::Id type = "PemServerMissionStatus"_id;
    float distance;
    int turn;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerMissionStatus& data) {
    return ar | data.distance | data.turn;
  }

  struct PemServerUpdateHand {
    static constexpr gf::Id type = "PemServerUpdateHand"_id;
    CardType card;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemServerUpdateHand& data) {
    return ar | data.card;
  }


  /*
   * client -> server
   */

  struct PemClientChoosePrisoner {
    static constexpr gf::Id type = "PemClientChoosePrisoner"_id;
    gf::Id member;
  };

  template<typename Archive>
  Archive operator|(Archive& ar, PemClientChoosePrisoner& data) {
    return ar | data.member;
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

#endif // PEM_PEM_PROTOCOL_H
