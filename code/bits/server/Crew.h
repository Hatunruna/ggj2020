#ifndef TMPGAME_CREW_H
#define TMPGAME_CREW_H

#include <vector>

#include "common/PemTypes.h"

namespace ggj {

  struct Crew {
    static int computeRebels(int32_t players);
    static int computeProtectors(int32_t players);

    static std::vector<CrewType> createCrew(int32_t players);
  };
}

#endif // TMPGAME_CREW_H
