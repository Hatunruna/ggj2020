#ifndef TMPGAME_DECK_H
#define TMPGAME_DECK_H

#include <cstdint>
#include <vector>

#include <gf/Random.h>

#include "common/PemTypes.h"

namespace ggj {

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

#endif // TMPGAME_DECK_H
