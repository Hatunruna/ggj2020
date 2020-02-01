#ifndef TMPGAME_CLIENT_PLAYER_DATA_H
#define TMPGAME_CLIENT_PLAYER_DATA_H

#include <gf/Id.h>
#include <string>

namespace ggj
{
    struct ClientPlayerData {
      gf::Id id;
      std::string name;
      bool captain = false;
    };
}


#endif // TMPGAME_CLIENT_PLAYER_DATA_H