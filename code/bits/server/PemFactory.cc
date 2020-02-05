#include "PemFactory.h"

#include "PemInstance.h"

namespace pem {

  std::unique_ptr<GameInstance> PemFactory::createInstance(const GameInstanceSettings& settings) {
    return std::make_unique<PemInstance>(settings.playersByTeam);
  }

}
