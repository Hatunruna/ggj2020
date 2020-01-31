#include "GameFactory.h"

namespace ggj {

  class PemFactory : public GameFactory {
  public:
    std::unique_ptr<GameInstance> createInstance(const GameInstanceSettings& settings) override;
  };

}


