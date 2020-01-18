#include "Scenes.h"

#include "Constants.h"

namespace ggj {

  Scenes::Scenes(ClientNetwork& network, gf::Path searchDir)
  : gf::SceneManager("tmpgame", InitialSize)
  , resources({ searchDir })
  , intro(*this, resources)
  , connection(*this, network, resources)
  , lobby(*this, network, resources)
  , room(*this, resources)
  , waiting(*this, resources)
  {

  }

}

