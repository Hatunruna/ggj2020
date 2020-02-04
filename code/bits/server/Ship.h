#ifndef TMPGAME_SHIP_H
#define TMPGAME_SHIP_H

#include <cstdint>
#include <map>
#include <vector>
#include <set>

#include <gf/Id.h>
#include "common/PemTypes.h"

namespace ggj {

//     Blocked,    // true
//     FalseAlarm, // false
//     Jammed,     // true

  enum class PlaceState : uint8_t {
    Broken,
    Working,
  };

  enum class ActionType : uint8_t {
    Explode,
    Repair,
  };

  struct Action {
    ActionType actionType;
    int remainingTurn;

    bool operator<(const Action& other) {
      return remainingTurn < other.remainingTurn && static_cast<uint8_t>(actionType) < static_cast<uint8_t>(other.actionType);
    }
  };

  struct ShipPlace {
    PlaceState publicState; // "fake" state, depending of current action
    PlaceState realState; // Real current state

    std::vector<Action> actions;

    // SetupJammer
    // PlaceState previous;
    // int jammed = 0;

    // // Block
    // int blocked = 0;

    // // FalseAlarm
    // int alarm = 0;

    // // FalseRepair
    // int repair = 0;

    // // PlaceBomb
    // int bomb = 0;

    // // Reinforce
    // int reinforcement = 0;

    // // Track
    // std::vector<gf::Id> trackers;

    std::set<gf::Id> members;
  };

  struct Ship {
    public:
      Ship(int32_t players);

      void addAction(PlaceType place, CardType card);
      void sortActions();
      void updateActions();

      std::map<PlaceType, bool> getPublicStates() const;
      float computeDistance() const;

      // std::map<PlaceType, bool> getState();

      // void addCrew(PlaceType type, gf::Id id);

      // void endOfActions();

      std::map<PlaceType, ShipPlace> places;
  };
}

#endif // TMPGAME_SHIP_H
