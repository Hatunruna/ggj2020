#include "Scenes.h"

#include "Constants.h"

namespace ggj {

  Scenes::Scenes()
  : gf::SceneManager("tmpgame", InitialSize)
  , intro(*this)
  {

  }

}

