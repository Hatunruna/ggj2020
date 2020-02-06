#ifndef PEM_CLIENT_PLAYER_DATA_H
#define PEM_CLIENT_PLAYER_DATA_H

#include <gf/Id.h>
#include <string>

namespace pem
{
    struct ClientPlayerData {
      ClientPlayerData()
      : id(gf::InvalidId)
      , name("")
      , jail(false)
      {}

      gf::Id id;
      std::string name;
      bool jail;
    };
}


#endif // PEM_CLIENT_PLAYER_DATA_H
