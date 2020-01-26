#include <gf/Random.h>

#include "bits/GameFactory.h"
#include "bits/GameInstance.h"
#include "bits/ServerNetwork.h"

namespace {

  class DummyGameInstance : public ggj::GameInstance {
  public:
    void update() override {

    }
  };

  class DummyGameFactory : public ggj::GameFactory {
  public:
    std::unique_ptr<ggj::GameInstance> createInstance(const ggj::GameInstanceSettings& settings) override {
      return std::make_unique<DummyGameInstance>();
    }
  };

}

int main() {
  DummyGameFactory dummy;

  ggj::GameSettings settings;
  settings.teamsMin = 2;
  settings.teamsMax = 4;
  settings.playersByTeamMin = 2;
  settings.playersByTeamMax = 5;

  gf::Random random;
  ggj::ServerNetwork network(random, dummy, settings);
  network.run();
  return EXIT_SUCCESS;
}
