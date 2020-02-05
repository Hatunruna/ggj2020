#ifndef PEM_DECK_H
#define PEM_DECK_H

#include <cstdint>
#include <vector>

#include <gf/Random.h>

#include "common/PemTypes.h"

namespace pem {

  class Deck {
  public:
    Deck(int32_t players, gf::Random& random);

    CardType pickProtectorCard();
    CardType pickRebelCard();

  private:
    std::size_t m_protectorIndex;
    std::vector<CardType> m_protectorsDeck;
    std::size_t m_rebelIndex;
    std::vector<CardType> m_rebelsDeck;
  };

}

#endif // PEM_DECK_H
