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

  inline std::string crewTypeString(CrewType type) {
    switch (type) {
    case CrewType::Rebel:
      return "Rebel";

    case CrewType::Protector:
      return "Protector";
    }

    assert(false);
    return "";
  }

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
    }

    assert(false);
    return "";
  }


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

  enum class PlaceState : uint8_t {
    Blocked,    // true
    FalseAlarm, // false
    Jammed,     // true
    Saboted,    // false
    Working,    // true
  };

  enum class ResolutionType : uint16_t {
    Examine,
    Hide,
    Block,
    Release,
  };

  struct Resolution {
    ResolutionType type;

    union {
      bool bomb; // Examine
      gf::Id member; // Hide
      PlaceType place; // Block
    };
  };

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

}

#endif // TMPGAME_PEM_TYPES_H
