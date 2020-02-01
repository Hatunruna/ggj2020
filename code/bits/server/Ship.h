#ifndef TMPGAME_SHIP_H
#define TMPGAME_SHIP_H

#include <cstdint>
#include <map>
#include <vector>
#include <set>

#include <gf/Id.h>
#include "common/PemTypes.h"

namespace ggj {

//     Blocked,    // true
//     FalseAlarm, // false
//     Jammed,     // true

  enum class PlaceState : uint8_t {
    Broken,
    Working,
  };

  struct ShipPlace {
    PlaceState state;

    // SetupJammer
    PlaceState previous;
    int jammed = 0;

    // Block
    bool blocked;

    // FalseAlarm
    int alarm = 0;

    // FalseRepair
    int repair = 0;

    // PlaceBomb
    int bomb = 0;

    // Reinforce
    int reinforcement = 0;

    // Track
    std::vector<gf::Id> trackers;

    std::set<gf::Id> members;
  };

  struct Ship {
    public:
      Ship(int32_t players);

      void changeState(PlaceType place ,PlaceState type);

      std::map<PlaceType, bool> getState();

      void addCrew(PlaceType type, gf::Id id);

      void clear();

      std::map<PlaceType, ShipPlace> places;
  };
}


#endif // TMPGAME_SHIP_H
