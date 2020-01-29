#include <gf/Random.h>

#include "bits/server/GameFactory.h"
#include "bits/server/GameInstance.h"
#include "bits/server/ServerNetwork.h"

namespace {

  class DummyGameInstance : public ggj::GameInstance {
  public:
    void start() override {

    }

    bool isFinished() override {
      return false;
    }

    void update(ggj::ServerPlayer& player, ggj::ProtocolBytes& bytes) override {

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
  settings.playersByTeamMin = 1;
  settings.playersByTeamMax = 5;

  gf::Random random;
  ggj::ServerNetwork network(random, dummy, settings);
  network.run();
  return EXIT_SUCCESS;
}
