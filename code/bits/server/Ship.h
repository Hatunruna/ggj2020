#ifndef TMPGAME_SHIP_H
#define TMPGAME_SHIP_H

#include <cstdint>
#include <map>
#include <vector>
#include <set>

#include <gf/Id.h>
#include "common/PemTypes.h"

namespace ggj {

  struct ShipPlace {
    PlaceState state;
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