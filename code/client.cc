#include <cstdlib>


#include "bits/Scenes.h"

#include "config.h"

int main() {
  ggj::Scenes scenes(TMPGAME_DATA_DIR);
  scenes.pushScene(scenes.intro);
  scenes.run();
  return EXIT_SUCCESS;
}
