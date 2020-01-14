#include "Scenes.h"

#include "Constants.h"

namespace ggj {

  Scenes::Scenes(gf::Path searchDir)
  : gf::SceneManager("tmpgame", InitialSize)
  , resources({ searchDir })
  , intro(*this, resources)
  {

  }

}

