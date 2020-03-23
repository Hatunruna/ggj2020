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
    , cardIndex(-1)
    {}

    std::array<CardType, MaxCards> cards;
    int32_t prison;
    PlaceType place; // Played place
    CardType card; // Played card
    int8_t cardIndex;
    CrewType crewType;
  };


}

#endif // PEM_MEMBER_H
