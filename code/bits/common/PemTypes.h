#ifndef TMPGAME_PEM_TYPES_H
#define TMPGAME_PEM_TYPES_H

#include <cassert>
#include <cstdint>
#include <string>

#include <gf/Id.h>

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
    None,
  };

  inline std::string placeTypeString(PlaceType type) {
    switch (type) {
    case PlaceType::Armory:
      return "Armory";

    case PlaceType::Bathroom:
      return "Bathroom";

    case PlaceType::CommunicationCenter:
      return "Communication Center";

    case PlaceType::Dormitory:
      return "Dormitory";

    case PlaceType::GreenHouse:
      return "Green House";

    case PlaceType::Infirmery:
      return "Infirmery";

    case PlaceType::LeftEngine:
      return "Left Engine";

    case PlaceType::LifeSupport:
      return "Life Support";

    case PlaceType::MainBridge:
      return "Main Bridge";

    case PlaceType::MidEngine:
      return "Mid Engine";

    case PlaceType::Navigation:
      return "Navigation";

    case PlaceType::Prison:
      return "Prison";

    case PlaceType::Refectory:
      return "Refectory";

    case PlaceType::RightEngine:
      return "Right Engine";

    case PlaceType::Storage:
      return "Storage";

    case PlaceType::None:
      assert(false);
      return "";
    }

    assert(false);
    return "";
  }


  enum class CardType : uint16_t {
    // protector
    Demine, // first to be used if the room is not working
    Examine, // define if we can know the cooldown reamining on the bomb if there is one
    Hide, // must be used first
    Reinforce1, // must be used first if the room is working
    Reinforce2, // must be used first if the room is working
    Repair, // must be used first if the room is not working
    Track, // no priority ?
    // common
    Block, // block for the enxt room -> no priority
    Release, // no priority
    // rebel
    FalseAlarm, // false alarm of a bomb no matter what
    FalseRepair1, // first to be used if the room is not working
    FalseRepair2, // first to be used if the room is not working
    PlaceBomb0, // last if the room is working
    PlaceBomb1, // last if the room is working
    PlaceBomb2, // last if the room is working
    SetupJammer, // next turn ?
  };

  // WORKING :
      // NOT USEABLE : Demine, Repair, FalseRepair1/2
  // NOT WORKING
      // NOT USEABLE : PlaceBomb1/2/3, Reinforce1/2/3, Examine


  // BLOCKED : NULL
  // FALSE ALARM : SABOTED
  // JAMMED  : WORKING
  // SABOTED : HIDE -> REPAIR -> FALSE REPAIR2 -> FALSE REPAIR1 ->             -> BLOCK/SETUP JAMMER/TRACK/FALSE ALARM
  // WORKING : HIDE -> EXAMINE -> REINFORCE2 -> REINFORCE1 -> BOMB0 -> BOMB1 -> BOMB2 ->       -> BLOCK/SETUP JAMMER/TRACK/FALSE ALARM

  enum class ResolutionType : uint16_t {
    Examine,
    Hide,
    Track,
    Block,
    Release,
  };

  struct Resolution {
    ResolutionType type;

    bool bomb; // Examine
    std::vector<gf::Id> members; // Hide, Track , Release (if there are more than 1 prisoneer)
  };

  template<typename Archive>
  Archive operator|(Archive& ar, Resolution& data) {
    return ar | data.type | data.bomb | data.members;
  }

  inline std::string cardTypeString(CardType type) {
    switch (type) {
    case CardType::Demine:
      return "Demine";

    case CardType::Examine:
      return "Examine";

    case CardType::Hide:
      return "Hide";

    case CardType::Reinforce1:
      return "Reinforce for 1 turn";

    case CardType::Reinforce2:
      return "Reinforce for 2 turn";

    case CardType::Repair:
      return "Repair";

    case CardType::Track:
      return "Track";

    case CardType::Block:
      return "Block";

    case CardType::Release:
      return "Release";

    case CardType::FalseAlarm:
      return "False alarm";

    case CardType::FalseRepair1:
      return "False Repair for 1 turn";

    case CardType::FalseRepair2:
      return "False Repair for 2 turn";

    case CardType::PlaceBomb0:
      return "Place immediate bomb";

    case CardType::PlaceBomb1:
      return "Place delayed bomb for 1 turn";

    case CardType::PlaceBomb2:
      return "Place delayed bomb for 2 turn";

    case CardType::SetupJammer:
      return "Setup Jammer";
    }

    assert(false);
    return "";
  }

  enum class GamePhase: uint8_t {
    CapitainElection,
    Action,
    Resolution,
    Meeting,
  };

  enum class VoteType : uint8_t {
    Captain,
    Prison,
  };

}

#endif // TMPGAME_PEM_TYPES_H
