#ifndef TMPGAME_GAME_FACTORY_H
#define TMPGAME_GAME_FACTORY_H

#include <memory>

#include "../common/GameSettings.h"

#include "GameInstance.h"

namespace ggj {

  class GameFactory {
  public:
    virtual ~GameFactory();
    virtual std::unique_ptr<GameInstance> createInstance(const GameInstanceSettings& settings) = 0;
  };

}

#endif // TMPGAME_GAME_FACTORY_H
