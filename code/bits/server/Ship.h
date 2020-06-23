#ifndef PEM_SHIP_H
#define PEM_SHIP_H

#include <cstdint>
#include <map>
#include <vector>
#include <set>

#include <gf/Id.h>
#include "common/PemTypes.h"

namespace pem {

  enum class ActionType : uint8_t {
    Alarm,
    Blocked,
    FakeFix,
    Demine,
    Reinforce,
    Explode,
    Repair,
    Examine,
  };

  struct Action {
    ActionType actionType;
    int remainingTurn;

    bool operator<(const Action& other) {
      bool turnGreater = remainingTurn > other.remainingTurn;
      bool actionLesser = remainingTurn == other.remainingTurn && static_cast<uint8_t>(actionType) < static_cast<uint8_t>(other.actionType);
      return turnGreater || actionLesser;
    }
  };

  enum class ReinforcedState {
    Reinforced,   // Protect from explode
    Used,         // The protection is gone. This state is reach when the reinforcement has already protected from a explosion
    None,         // Default state (no protection)
  };

  struct ShipPlace {
    std::vector<Action> actions;

    // State of place
    bool broken = false;

    // Alarm
    bool alarm = false;

    // Blocked
    bool blocked = false;

    // False Repair
    bool fakeFix = false;

    // Reinforced
    ReinforcedState reinforced = ReinforcedState::None;

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

      /**
       * Sort the actions by highest turn then by lower ActionType.
       * We place the highest actions in first to handle the specific
       * case like reinforcement: we need to apply a eventually reinforcement
       * before an explosion
       */
      void sortActions();
      void updateActions();

      std::map<PlaceType, bool> getPublicStates() const;
      float computeDistance() const;
      std::vector<std::string> getPlaceStateStrings(PlaceType place) const;

      std::map<PlaceType, ShipPlace> places;

    private:
      std::vector<Action>::iterator getLastTurnAction(std::vector<Action> &actions);
  };
}

#endif // PEM_SHIP_H
