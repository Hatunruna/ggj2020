#ifndef TMPGAME_SHIP_H
#define TMPGAME_SHIP_H

#include <cstdint>
#include <map>
#include <vector>

#include "common/PemTypes.h"

namespace ggj {

  struct ShipPlace{
    PlaceState state;
  };
  struct Ship {
    public:
      Ship(int32_t players);
      std::map<PlaceType,PlaceState> ShipRooms;
      void changeState(PlaceType place ,PlaceState type);
  };
}


#endif // TMPGAME_SHIP_H