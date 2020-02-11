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

      case CardType::Demine:
        action.actionType = ActionType::Demine;
        action.remainingTurn = 0;
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
      auto partition = getLastActionIterator(place.second.actions);

      // Apply actions
      auto &actions = place.second.actions;
      for (auto it = actions.begin(); it != partition;++it) {
        switch (it->actionType) {
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

    return static_cast<float>(workingRoom) / static_cast<float>(places.size());
  }

  std::vector<Action>::iterator Ship::getLastActionIterator(std::vector<Action> &actions) {
    auto partition = std::partition(actions.begin(), actions.end(), [](const auto &action) {
      return action.remainingTurn == 0;
    });
    return partition;
  }
}
