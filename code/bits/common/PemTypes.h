#ifndef TMPGAME_PEM_TYPES_H
#define TMPGAME_PEM_TYPES_H

#include <cstdint>

namespace ggj {

  enum class CrewType : uint8_t {
    Rebel,
    Protector,
  };

  enum class PlaceType : uint16_t {
    Armory,
    Bathroom,
    CommunicationCenter,
    Dormitory,
    GreenHouse,
    Infirmery,
    LeftEngine,
    LifeSupport,
    MainBridge,
    MidEngine,
    Navigation,
    Prison,
    Refectory,
    RightEngine,
    Storage,
  };

  enum class CardType : uint16_t {
    // protector
    Demine,
    Examine,
    Hide,
    Reinforce1,
    Reinforce2,
    Repair,
    Track,
    // common
    Block,
    Release,
    // rebel
    FalseAlarm,
    FalseRepair1,
    FalseRepair2,
    PlaceBomb0,
    PlaceBomb1,
    PlaceBomb2,
    SetupJammer,
  };

  enum class PlaceState : uint8_t{
    Blocked, // true
    FalseAlarm, // false
    Jammed, // true
    Saboted, // false
    Working, // true
  };

}

#endif // TMPGAME_PEM_TYPES_H
