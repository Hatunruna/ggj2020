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

      case CardType::FalseAlarm:
        action.actionType = ActionType::AlarmStart;
        action.remainingTurn = 0;
        actions.push_back(action);
        action.actionType = ActionType::AlarmStop;
        action.remainingTurn = 1;
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
      // Select the action to do
      // NOTE: Useless since the vector is ordered but it's simpler
      auto partition = getLastActionIterator(place.second.actions);

      // Apply actions
      auto &actions = place.second.actions;
      for (auto it = actions.begin(); it != partition;++it) {
        switch (it->actionType) {
          case ActionType::AlarmStart:
            gf::Log::debug("(Ship) The place '%s' triggers a false alarm\n", placeTypeString(place.first).c_str());
            place.second.alarm = true;
            break;

          case ActionType::AlarmStop:
            gf::Log::debug("(Ship) The place '%s' end of a false alarm\n", placeTypeString(place.first).c_str());
            place.second.alarm = false;
            break;

          case ActionType::Demine: {
            gf::Log::debug("(Ship) The place '%s' has been demine\n", placeTypeString(place.first).c_str());
            auto itRemove = std::remove_if(actions.begin(), actions.end(), [](const auto &action) {
              return action.actionType == ActionType::Explode;
            });

            actions.erase(itRemove, actions.end());
            // Update the last action if needed
            partition = getLastActionIterator(place.second.actions);
            break;
          }

          case ActionType::Explode:
            gf::Log::debug("(Ship) The place '%s' has explode\n", placeTypeString(place.first).c_str());
            place.second.broken = true;
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
      const auto &state = place.second;

      if (state.alarm) {
        states.emplace(place.first, false);
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

  std::vector<Action>::iterator Ship::getLastActionIterator(std::vector<Action> &actions) {
    auto partition = std::partition(actions.begin(), actions.end(), [](const auto &action) {
      return action.remainingTurn == 0;
    });
    return partition;
  }
}
