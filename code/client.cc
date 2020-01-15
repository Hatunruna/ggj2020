#include <cstdlib>

#include <gf/Log.h>

#include "bits/Communication.h"
#include "bits/Scenes.h"

#include "config.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    gf::Log::error("Usage: tmpgame <hostname> <player>\n");
    return EXIT_FAILURE;
  }

  gf::Log::info("Connecting to server...\n");

  ggj::Communication comm(argv[1], argv[2]);

  gf::Log::info("Running...\n");

  ggj::Scenes scenes(TMPGAME_DATA_DIR);
  scenes.pushScene(scenes.intro);
  scenes.run();
  return EXIT_SUCCESS;
}
