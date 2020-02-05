#ifndef PEM_CREW_H
#define PEM_CREW_H

#include <vector>

#include "common/PemTypes.h"

namespace pem {

  struct Crew {
    static int computeRebels(int32_t players);
    static int computeProtectors(int32_t players);

    static std::vector<CrewType> createCrew(int32_t players);
  };
}

#endif // PEM_CREW_H
