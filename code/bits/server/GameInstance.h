#ifndef PEM_GAME_INSTANCE_H
#define PEM_GAME_INSTANCE_H

#include "ServerGroup.h"

namespace pem {

  class GameInstance : public ServerGroup {
  public:
    virtual void start() = 0;
    virtual bool isFinished() = 0;
  };

}

#endif // PEM_GAME_INSTANCE_H
