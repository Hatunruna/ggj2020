#ifndef PEM_PEM_FACTORY_H
#define PEM_PEM_FACTORY_H

#include "GameFactory.h"

namespace pem {

  class PemFactory : public GameFactory {
  public:
    std::unique_ptr<GameInstance> createInstance(const GameInstanceSettings& settings) override;
  };

}

#endif // PEM_PEM_FACTORY_H
