#include "Crew.h"

#include <cassert>

namespace pem {

  int Crew::computeRebels(int32_t players) {
    assert(4 >= players && players <= 8);

    if (players <= 6) {
      return 1;
    }

    return 2;
  }

  int Crew::computeProtectors(int32_t players) {
    return players - computeRebels(players);
  }

  std::vector<CrewType> Crew::createCrew(int32_t players) {
    std::vector<CrewType> crew;

    int32_t protectors = computeProtectors(players);

    for (int32_t i = 0; i < protectors; ++i) {
      crew.push_back(CrewType::Protector);
    }

    int32_t rebels = computeRebels(players);

    for (int32_t i = 0; i < rebels; ++i) {
      crew.push_back(CrewType::Rebel);
    }

    assert(crew.size() == static_cast<unsigned>(players));

    return crew;
  }


}


