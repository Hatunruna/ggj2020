#include <cstdlib>

#include <gf/Log.h>

#include "bits/client/ClientNetwork.h"
#include "bits/client/Scenes.h"

#include "config.h"

int main() {
  ggj::ClientNetwork network;

  ggj::Scenes scenes(network, TMPGAME_DATA_DIR);
  scenes.pushScene(scenes.intro);
  scenes.run();
  return EXIT_SUCCESS;
}
