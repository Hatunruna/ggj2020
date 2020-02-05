#ifndef PEM_CLIENT_PLAYER_DATA_H
#define PEM_CLIENT_PLAYER_DATA_H

#include <gf/Id.h>
#include <string>

namespace pem
{
    struct ClientPlayerData {
      gf::Id id;
      std::string name;
      bool captain = false;
      bool jail = false;
    };
}


#endif // PEM_CLIENT_PLAYER_DATA_H
