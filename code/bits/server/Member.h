#ifndef PEM_MEMBER_H
#define PEM_MEMBER_H

#include "common/Constants.h"
#include "common/PemTypes.h"

namespace pem {

  struct Member {
    // std::array<CardType, MaxCards> cards;
    int32_t prison = 0;
    // bool voted = false;
    // bool captain = false;
    // bool released = true;
    PlaceType place; // Played place
    CardType card; // Played card
    // CrewType type;
  };


}

#endif // PEM_MEMBER_H
