#include "PemInstance.h"

namespace ggj {
  PemInstance::PemInstance(int32_t players)
  : m_players(players)
  , m_deck(players, m_random)
  {
  }

  void PemInstance::start() {

  }

  bool PemInstance::isFinished() {
    return false;
  }

  void PemInstance::update(ServerPlayer& player, ProtocolBytes& bytes) {

  }

}


