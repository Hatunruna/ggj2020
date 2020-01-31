#ifndef TMPGAME_PEM_TYPES_H
#define TMPGAME_PEM_TYPES_H

#include <cstdint>

namespace ggj {

  enum PlaceType : uint16_t {
    Armory,
    Bathroom,
    CommunicationCenter,
    Deck,
    Dormitory,
    GreenHouse,
    Infirmery,
    LeftEngine,
    LifeSupport,
    MidEngine,
    Navigation,
    Prison,
    Refectory,
    RightEngine,
    Storage,
  };

  enum CardType : uint16_t {
    // repair
    Repair,
    Hide,
    Reinforce1,
    Reinforce2,
    Track,
    Examine,
    Demine,
    // common
    Block,
    Release,
    // destroy
    PlaceBomb0,
    PlaceBomb1,
    PlaceBomb2,
    FalseRepair1,
    FalseRepair2,
    FalseAlarm,
    SetupJammer,
  };


}

#endif // TMPGAME_PEM_TYPES_H
