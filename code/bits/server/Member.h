#ifndef PEM_MEMBER_H
#define PEM_MEMBER_H

#include "common/Constants.h"
#include "common/PemTypes.h"

namespace pem {

  struct Member {
    Member()
    : prison(0)
    , place(PlaceType::None)
    , card(CardType::None)
    {}

    // std::array<CardType, MaxCards> cards;
    int32_t prison = 0;
    PlaceType place; // Played place
    CardType card; // Played card
    CrewType crewType;
  };


}

#endif // PEM_MEMBER_H
