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

  void Ship::changeState(PlaceType type ,PlaceState state){
    auto it = places.find(type);
    assert(it != places.end());
    it->second.state = state;
  }

  void Ship::addCrew(PlaceType type, gf::Id id){
    auto it = places.find(type);
    assert(it != places.end());
    it->second.members.insert(id);
  }

  void Ship::clear() {
    for (auto& kv : places) {
      kv.second.members.clear();
    }
  }

  std::map<PlaceType, bool> Ship::getState(){
    std::map<PlaceType, bool> state;

    for (auto& kv : places) {
      if(kv.second.state == PlaceState::FalseAlarm || kv.second.state == PlaceState::Saboted) {
        state.insert({ kv.first, false });
      }else{
        state.insert({ kv.first, true });
      }

      // TODO : Jammed
    }

    return state;
  }
}