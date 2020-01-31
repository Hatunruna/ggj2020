#include <gf/Random.h>

#include "bits/server/GameFactory.h"
#include "bits/server/GameInstance.h"
#include "bits/server/ServerNetwork.h"

#include "bits/server/PemFactory.h"

int main() {
  ggj::PemFactory factory;

  ggj::GameSettings settings;
  settings.teamsMin = 1;
  settings.teamsMax = 1;
  settings.playersByTeamMin = 4;
  settings.playersByTeamMax = 8;

  gf::Random random;
  ggj::ServerNetwork network(random, factory, settings);
  network.run();
  return EXIT_SUCCESS;
}
