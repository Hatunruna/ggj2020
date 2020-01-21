#ifndef TMPGAME_GAME_INSTANCE_H
#define TMPGAME_GAME_INSTANCE_H

namespace ggj {

  class GameInstance {
  public:
    virtual ~GameInstance();
    virtual void update() = 0;
  };

}

#endif // TMPGAME_GAME_INSTANCE_H
