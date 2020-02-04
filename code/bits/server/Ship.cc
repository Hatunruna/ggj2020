#include "Ship.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <map>
#include <utility>
#include <vector>

#include <gf/Log.h>

namespace ggj {

  namespace {
    ShipPlace createPlace(PlaceState state){
      ShipPlace place;
      place.publicState = state;
      place.realState = state;

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
        { PlaceType::LifeSupport,         createPlace(PlaceState::Working) },
        { PlaceType::MidEngine,           createPlace(PlaceState::Working) },
        { PlaceType::Bathroom,            createPlace(PlaceState::Working) },
        { PlaceType::Dormitory,           createPlace(PlaceState::Working) },
        { PlaceType::Armory,              createPlace(PlaceState::Working) },
        { PlaceType::GreenHouse,          createPlace(PlaceState::Working) },
      };

      // if (players > 4){
        ship.insert({ PlaceType::LifeSupport,   createPlace(PlaceState::Working)});
        ship.insert({ PlaceType::MidEngine,     createPlace(PlaceState::Working)});
      // }

      // if (players > 5){
        ship.insert({ PlaceType::Bathroom,      createPlace(PlaceState::Working)});
        ship.insert({ PlaceType::Dormitory,     createPlace(PlaceState::Working)});
      // }

      // if (players > 6){
        ship.insert({ PlaceType::Armory,        createPlace(PlaceState::Working)});
        ship.insert({ PlaceType::GreenHouse,    createPlace(PlaceState::Working)});
      // }

      return ship;
    }

    Action createAction(CardType card) {
      assert(card != CardType::None);

      Action action;
      switch (card) {
      case CardType::Repair:
        action.actionType = ActionType::Repair;
        action.remainingTurn = 0;
        break;

      case CardType::PlaceBomb0:
        action.actionType = ActionType::Explode;
        action.remainingTurn = 0;
        break;

      case CardType::PlaceBomb1:
        action.actionType = ActionType::Explode;
        action.remainingTurn = 1;
        break;

      case CardType::PlaceBomb2:
        action.actionType = ActionType::Explode;
        action.remainingTurn = 2;
        break;

      default:
        assert(false);
        break;
      }

      return action;
    }
  }

  Ship::Ship(int32_t players)
  : places(createShip(players)) {
  }

  void Ship::addAction(PlaceType place, CardType card) {
    assert(place != PlaceType::None && card != CardType::None);
    auto &placeEntry = places.at(place);
    placeEntry.actions.push_back(createAction(card));
  }

  void Ship::sortActions() {
    for (auto &entry: places) {
      auto &place = entry.second;

      std::sort(place.actions.begin(), place.actions.end());
    }
  }

  void Ship::updateActions() {
    for (auto &place: places) {
      // Select the action to do
      // NOTE: Useless since the vector is ordered but it's simpler
      auto partition = std::partition(place.second.actions.begin(), place.second.actions.end(), [](const auto &action) {
        return action.remainingTurn == 0;
      });

      // Apply actions
      auto &actions = place.second.actions;
      for (auto it = actions.begin(); it != partition; ++it) {
        switch (it->actionType) {
          case ActionType::Explode:
            gf::Log::debug("(Ship) The place '%s' has explode\n", placeTypeString(place.first).c_str());
            place.second.publicState = PlaceState::Broken;
            place.second.realState = PlaceState::Broken;
            break;

          case ActionType::Repair:
            gf::Log::debug("(Ship) The place '%s' has been repair\n", placeTypeString(place.first).c_str());
            place.second.publicState = PlaceState::Working;
            place.second.realState = PlaceState::Working;
            break;

          default:
            assert(false);
            break;
        }
      }

      // Remove old actions
      actions.erase(actions.begin(), partition);

      // Update remaining turns
      for (auto &action: actions) {
        --action.remainingTurn;
      }
    }
  }

  std::map<PlaceType, bool> Ship::getPublicStates() const {
    std::map<PlaceType, bool> states;

    for (const auto &place: places) {
      states.emplace(place.first, place.second.publicState == PlaceState::Working);
    }

    return states;
  }

  float Ship::computeDistance() const {
    int workingRoom = std::count_if(places.begin(), places.end(), [](const auto &entry) {
      return entry.second.realState;
    });

    return static_cast<float>(workingRoom) * 2.0f / static_cast<float>(places.size());
  }

  // void Ship::addCrew(PlaceType type, gf::Id id){
  //   places.at(type).members.insert(id);
  // }

  // void Ship::endOfActions() {
  //   for (auto& kv : places) {
  //     auto & place = kv.second;

  //     if (place.jammed == 2) {
  //       place.previous = place.state;
  //     }

  //     if (place.jammed > 0) {
  //       --place.jammed;
  //     }

  //     if (place.bomb == 1) {
  //       if (place.reinforcement == 0) {
  //         // bomb explodes
  //         place.state = PlaceState::Broken;
  //       } else {
  //         place.reinforcement = 0;
  //       }
  //     }

  //     if (place.bomb > 0) {
  //       --place.bomb;
  //     }

  //     if (place.reinforcement > 0) {
  //       --place.reinforcement;
  //     }

  //     if (place.blocked > 0) {
  //       --place.blocked;
  //     }

  //     if (place.alarm > 0) {
  //       --place.alarm;
  //     }

  //     if (place.repair > 0) {
  //       --place.repair;
  //     }
  //   }
  // }

  // std::map<PlaceType, bool> Ship::getState() {
  //   std::map<PlaceType, bool> state;

  //   for (auto& kv : places) {
  //     auto& place = kv.second;

  //     if (place.state == PlaceState::Working) {
  //       bool res = true;

  //       if (place.jammed > 0) {
  //         res = (place.previous == PlaceState::Working);
  //       } else if (place.alarm > 0) {
  //         res = false;
  //       }

  //       state.emplace(kv.first, res);
  //     } else {
  //       assert(place.state == PlaceState::Broken);
  //       bool res = false;

  //       if (place.jammed > 0) {
  //         res = (place.previous == PlaceState::Working);
  //       } else if (place.repair > 0) {
  //         res = true;
  //       }

  //       state.emplace(kv.first, res);
  //     }
  //   }

  //   return state;
  // }
}
