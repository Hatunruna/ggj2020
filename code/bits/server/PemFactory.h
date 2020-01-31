#ifndef TMPGAME_PEM_FACTORY_H
#define TMPGAME_PEM_FACTORY_H

#include "GameFactory.h"

namespace ggj {

  class PemFactory : public GameFactory {
  public:
    std::unique_ptr<GameInstance> createInstance(const GameInstanceSettings& settings) override;
  };

}

#endif // TMPGAME_PEM_FACTORY_H
