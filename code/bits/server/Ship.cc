#include "Ship.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <map>
#include <utility>
#include <vector>

#include <gf/Log.h>

namespace pem {

  namespace {
    std::map<PlaceType,ShipPlace> createShip(int32_t players){
      std::map<PlaceType,ShipPlace> ship = {
        { PlaceType::Infirmery,           ShipPlace() },
        { PlaceType::CommunicationCenter, ShipPlace() },
        { PlaceType::Navigation,          ShipPlace() },
        { PlaceType::Prison,              ShipPlace() },
        { PlaceType::Refectory,           ShipPlace() },
        { PlaceType::RightEngine,         ShipPlace() },
        { PlaceType::LeftEngine,          ShipPlace() },
        { PlaceType::Storage,             ShipPlace() },
        { PlaceType::MainBridge,          ShipPlace() },
        { PlaceType::LifeSupport,         ShipPlace() },
        { PlaceType::MidEngine,           ShipPlace() },
        { PlaceType::Bathroom,            ShipPlace() },
        { PlaceType::Dormitory,           ShipPlace() },
        { PlaceType::Armory,              ShipPlace() },
        { PlaceType::GreenHouse,          ShipPlace() },
      };

      // if (players > 4){
        ship.insert({ PlaceType::LifeSupport,   ShipPlace() });
        ship.insert({ PlaceType::MidEngine,     ShipPlace() });
      // }

      // if (players > 5){
        ship.insert({ PlaceType::Bathroom,      ShipPlace() });
        ship.insert({ PlaceType::Dormitory,     ShipPlace() });
      // }

      // if (players > 6){
        ship.insert({ PlaceType::Armory,        ShipPlace() });
        ship.insert({ PlaceType::GreenHouse,    ShipPlace() });
      // }

      return ship;
    }

    void createAction(CardType card, std::vector<Action> &actions) {
      assert(card != CardType::None);

      Action action;
      switch (card) {
      case CardType::Block:
        action.actionType = ActionType::Blocked;
        action.remainingTurn = 0;
        break;

      case CardType::Demine:
        action.actionType = ActionType::Demine;
        action.remainingTurn = 0;
        break;

      case CardType::Examine:
        action.actionType = ActionType::Examine;
        action.remainingTurn = 0;
        break;

      case CardType::FalseAlarm:
        action.actionType = ActionType::Alarm;
        action.remainingTurn = 1;
        break;

      case CardType::FalseRepair1:
        action.actionType = ActionType::FakeFix;
        action.remainingTurn = 1;
        break;

      case CardType::FalseRepair2:
        action.actionType = ActionType::FakeFix;
        action.remainingTurn = 2;
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

      case CardType::Repair:
        action.actionType = ActionType::Repair;
        action.remainingTurn = 0;
        break;

      default:
        assert(false);
        break;
      }

      actions.push_back(action);
    }
  }

  Ship::Ship(int32_t players)
  : places(createShip(players)) {
  }

  void Ship::addAction(PlaceType place, CardType card) {
    assert(place != PlaceType::None && card != CardType::None);
    auto &placeEntry = places.at(place);
    createAction(card, placeEntry.actions);
  }

  void Ship::sortActions() {
    for (auto &entry: places) {
      auto &place = entry.second;

      std::sort(place.actions.begin(), place.actions.end());
    }
  }

  void Ship::updateActions() {
    for (auto &place: places) {
      // Apply actions
      auto &actions = place.second.actions;
      for (unsigned i = 0; i < actions.size(); ++i) {
        auto &action = actions.at(i);
        switch (action.actionType) {
          case ActionType::Alarm:
            if (action.remainingTurn == 0) {
              gf::Log::debug("(Ship) The place '%s' end of a false alarm\n", placeTypeString(place.first).c_str());
              place.second.alarm = false;
            }
            else {
              gf::Log::debug("(Ship) The place '%s' triggers a false alarm\n", placeTypeString(place.first).c_str());
              place.second.alarm = true;
            }
            break;

          case ActionType::Demine: {
            gf::Log::debug("(Ship) The place '%s' has been demine\n", placeTypeString(place.first).c_str());
            auto itRemove = std::remove_if(actions.begin(), actions.end(), [](const auto &action) {
              return action.actionType == ActionType::Explode;
            });

            actions.erase(itRemove, actions.end());
            break;
          }

          case ActionType::Examine:
            gf::Log::debug("(Ship) The place '%s' has been examine\n", placeTypeString(place.first).c_str());
            // Nothing to do, the action is handle after the place update
            break;

          case ActionType::Explode:
            // Handle the action only when its turn
            if (action.remainingTurn != 0) {
              break;
            }

            gf::Log::debug("(Ship) The place '%s' has explode\n", placeTypeString(place.first).c_str());
            place.second.broken = true;
            break;

          case ActionType::FakeFix:
            // If the room is OK, nothing to do
            if (!place.second.broken) {
              break;
            }

            if (action.remainingTurn == 0) {
              place.second.fakeFix = false;
            }
            else {
              place.second.fakeFix = true;
            }

            gf::Log::debug("(Ship) The place '%s' has been fake fixed\n", placeTypeString(place.first).c_str());
            break;

          case ActionType::Blocked:
            gf::Log::debug("(Ship) The place '%s' is blocked\n", placeTypeString(place.first).c_str());
            break;

          case ActionType::Repair:
            gf::Log::debug("(Ship) The place '%s' has been repair\n", placeTypeString(place.first).c_str());
            place.second.broken = false;
            break;

          default:
            assert(false);
            break;
        }
      }

      // Remove old actions
      actions.erase(actions.begin(), getLastTurnAction(actions));

      // Update remaining turns
      for (auto &action: actions) {
        --action.remainingTurn;
      }
    }
  }

  std::map<PlaceType, bool> Ship::getPublicStates() const {
    std::map<PlaceType, bool> states;

    for (const auto &place: places) {
      const auto &state = place.second;

      if (state.alarm) {
        states.emplace(place.first, false);
      }
      else if (state.fakeFix) {
        states.emplace(place.first, true);
      }
      else {
        states.emplace(place.first, !state.broken);
      }
    }

    return states;
  }

  float Ship::computeDistance() const {
    int workingRoom = std::count_if(places.begin(), places.end(), [](const auto &entry) {
      return entry.second.broken;
    });

    return static_cast<float>(workingRoom) / static_cast<float>(places.size());
  }

  std::vector<std::string> Ship::getPlaceStateStrings(PlaceType place) const {
    std::vector<std::string> stateStrings;

    const ShipPlace state = places.at(place);

    if (state.alarm) {
      stateStrings.push_back("The place is under false alarm");
    }
    else if (state.fakeFix) {
      stateStrings.push_back("The repair is fake");
    }

    if (state.broken) {
      stateStrings.push_back("The place is broken");
    }
    else if (stateStrings.size() == 0) {
      stateStrings.push_back("Everything's in order");
    }
    else {
      stateStrings.push_back("The room is fonctional");
    }

    return stateStrings;
  }

  std::vector<Action>::iterator Ship::getLastTurnAction(std::vector<Action> &actions) {
    auto partition = std::partition(actions.begin(), actions.end(), [](const auto &action) {
      return action.remainingTurn == 0;
    });
    return partition;
  }
}
