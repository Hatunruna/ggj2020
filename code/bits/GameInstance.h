#ifndef TMPGAME_GAME_INSTANCE_H
#define TMPGAME_GAME_INSTANCE_H

#include "ProtocolBytes.h"
#include "ServerGroup.h"

namespace ggj {

  class GameInstance : public ServerGroup {
  public:
    virtual void start() = 0;
  };

}

#endif // TMPGAME_GAME_INSTANCE_H
