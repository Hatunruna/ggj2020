#include "PemInstance.h"

namespace ggj {
  PemInstance::PemInstance(int32_t players)
  : m_players(players)
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


