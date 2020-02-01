#include <cstdlib>

#include <gf/Log.h>

#include <SFML/Audio.hpp>

#include "bits/client/ClientNetwork.h"
#include "bits/client/Scenes.h"
#include "bits/client/Singletons.h"

#include "config.h"

static constexpr float MaxVolume = 100.0f;

int main() {
  ggj::ClientNetwork network;

  // Singletons
  gf::SingletonStorage<ggj::ResourceManager> storageForResourceManager(ggj::gResourceManager);
  ggj::gResourceManager().addSearchDir(TMPGAME_DATA_DIR);

  // Background music
  ggj::gBackgroundMusic.setBuffer(ggj::gResourceManager().getSound("audio/main_theme.ogg"));
  ggj::gBackgroundMusic.setLoop(true);
  ggj::gBackgroundMusic.setVolume(0.0f);
  //ggj::gBackgroundMusic.play();

  ggj::Scenes scenes(network, TMPGAME_DATA_DIR);
  scenes.pushScene(scenes.intro);
  scenes.run();
  return EXIT_SUCCESS;
}
