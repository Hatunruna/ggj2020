#ifndef PEM_GAME_FACTORY_H
#define PEM_GAME_FACTORY_H

#include <memory>

#include "common/GameSettings.h"

#include "GameInstance.h"

namespace pem {

  class GameFactory {
  public:
    virtual ~GameFactory();
    virtual std::unique_ptr<GameInstance> createInstance(const GameInstanceSettings& settings) = 0;
  };

}

#endif // PEM_GAME_FACTORY_H
