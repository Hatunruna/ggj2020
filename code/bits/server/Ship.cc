#include <cassert>
#include <cstdint>
#include <map>
#include <utility>
#include <vector>

#include "Ship.h"

namespace ggj {

  namespace {
    ShipPlace createPlace(PlaceState state){
      ShipPlace place;
      place.state = state;

      return place;
    }

    std::map<PlaceType,ShipPlace> createShip(int32_t players){
      std::map<PlaceType,ShipPlace> ship = {
        { PlaceType::Infirmery,           createPlace(PlaceState::Working) },
        { PlaceType::CommunicationCenter, createPlace(PlaceState::Working) },
        { PlaceType::Navigation,          createPlace(PlaceState::Working) },
        { PlaceType::Prison,              createPlace(PlaceState::Working) },
        { PlaceType::Refectory,           createPlace(PlaceState::Working) },
        { PlaceType::RightEngine,         createPlace(PlaceState::Working) },
        { PlaceType::LeftEngine,          createPlace(PlaceState::Working) },
        { PlaceType::Storage,             createPlace(PlaceState::Working) },
        { PlaceType::MainBridge,          createPlace(PlaceState::Working) },
      };

      if (players > 4){
        ship.insert({ PlaceType::LifeSupport,   createPlace(PlaceState::Working)});
        ship.insert({ PlaceType::MidEngine,     createPlace(PlaceState::Working)});
      }

      if (players > 5){
        ship.insert({ PlaceType::Bathroom,      createPlace(PlaceState::Working)});
        ship.insert({ PlaceType::Dormitory,     createPlace(PlaceState::Working)});
      }

      if (players > 6){
        ship.insert({ PlaceType::Armory,        createPlace(PlaceState::Working)});
        ship.insert({ PlaceType::GreenHouse,    createPlace(PlaceState::Working)});
      }

      return ship;
    }

  }

  Ship::Ship(int32_t players)
  : places(createShip(players))
  {
  }

  void Ship::changeState(PlaceType type, PlaceState state) {
    places[type].state = state;
  }

  void Ship::addCrew(PlaceType type, gf::Id id){
    places[type].members.insert(id);
  }

  void Ship::endOfActions() {
    for (auto& kv : places) {
      auto & place = kv.second;

      if (place.jammed == 2) {
        place.previous = place.state;
      }

      if (place.jammed > 0) {
        --place.jammed;
      }

      if (place.bomb == 1) {
        if (place.reinforcement == 0) {
          // bomb explodes
          place.state = PlaceState::Broken;
        } else {
          place.reinforcement = 0;
        }
      }

      if (place.bomb > 0) {
        --place.bomb;
      }

      if (place.reinforcement > 0) {
        --place.reinforcement;
      }

      if (place.blocked > 0) {
        --place.blocked;
      }

      if (place.alarm > 0) {
        --place.alarm;
      }

      if (place.repair > 0) {
        --place.repair;
      }
    }
  }

  std::map<PlaceType, bool> Ship::getState() {
    std::map<PlaceType, bool> state;

    for (auto& kv : places) {
      auto& place = kv.second;

      if (place.state == PlaceState::Working) {
        bool res = true;

        if (place.jammed > 0) {
          res = (place.previous == PlaceState::Working);
        } else if (place.alarm > 0) {
          res = false;
        }

        state.emplace(kv.first, res);
      } else {
        assert(place.state == PlaceState::Broken);
        bool res = false;

        if (place.jammed > 0) {
          res = (place.previous == PlaceState::Working);
        } else if (place.repair > 0) {
          res = true;
        }

        state.emplace(kv.first, res);
      }
    }

    return state;
  }
}
