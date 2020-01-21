#include <gf/Random.h>

#include "bits/ServerNetwork.h"

int main() {
  gf::Random random;
  ggj::ServerNetwork network(random);
  network.run();
  return EXIT_SUCCESS;
}
