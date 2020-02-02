#ifndef TMPGAME_MEMBER_H
#define TMPGAME_MEMBER_H

#include "common/Constants.h"
#include "common/PemTypes.h"

namespace ggj {

  struct Member {
    std::array<CardType, MaxCards> cards;
    int32_t prison = 0;
    bool voted = false;
    bool captain = false;
    bool released = true;
    PlaceType place;
    CardType card;
    CrewType type;
  };


}

#endif // TMPGAME_MEMBER_H
