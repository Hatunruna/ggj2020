#include <cassert>
#include <cstdint>
#include <map>
#include <utility>
#include <vector>

#include "Ship.h"

namespace ggj {
    
    std::map<PlaceType,PlaceState> BaseShip = {
      { PlaceType::Infirmery,           PlaceState::Working       }, 
      { PlaceType::CommunicationCenter, PlaceState::Working       },
      { PlaceType::MainBridge,          PlaceState::Working       },
      { PlaceType::Navigation,          PlaceState::Working       },
      { PlaceType::Prison,              PlaceState::Working       },
      { PlaceType::Refectory,           PlaceState::Working       },
      { PlaceType::RightEngine,         PlaceState::Working       },
      { PlaceType::LeftEngine,          PlaceState::Working       },
      { PlaceType::Storage,             PlaceState::Working       },
    };

    std::map<PlaceType,PlaceState> createShip(int32_t players){

      if(players > 4){
        BaseShip.insert({ PlaceType::LifeSupport,   PlaceState::Working});
        BaseShip.insert({ PlaceType::MidEngine,     PlaceState::Working});
      }
      if(players > 5){
        BaseShip.insert({ PlaceType::Bathroom,      PlaceState::Working});
        BaseShip.insert({ PlaceType::Dormitory,     PlaceState::Working});
      }
      if(players > 6){
        BaseShip.insert({ PlaceType::Armory,        PlaceState::Working});
        BaseShip.insert({ PlaceType::GreenHouse,    PlaceState::Working});
      }
      return BaseShip;
    }

    Ship::Ship(int32_t players)
    : ShipRooms(createShip(players))
    {
    }

    void Ship::changeState(PlaceType type ,PlaceState state){
      auto it = ShipRooms.find(type);
      assert(it != ShipRooms.end());
      it->second = state;
    }
}