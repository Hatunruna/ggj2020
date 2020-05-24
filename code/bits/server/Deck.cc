#include "Deck.h"

#include <cassert>
#include <algorithm>

#include <gf/ArrayRef.h>

#include "Crew.h"

namespace pem {

  namespace {

    struct DeckData {
      CardType card;
      int count;
    };

    constexpr DeckData ProtectorDeckConfig[] = {
      // { CardType::Demine,      2 },
      // { CardType::Examine,     5 },
      // { CardType::Hide,        2 },
      // { CardType::Reinforce1,  5 },
      // { CardType::Reinforce2,  2 },
      // { CardType::Repair,     10 },
      // { CardType::Track,       2 },
      // { CardType::Block,       2 },
      // { CardType::Release,     1 },
      { CardType::Demine,     15 },
      { CardType::Repair,     15 },
      { CardType::Release,    1  },
    };

    constexpr DeckData RebelDeckConfig[] = {
      // { CardType::FalseAlarm,    3 },
      // { CardType::FalseRepair1,  2 },
      // { CardType::FalseRepair2,  2 },
      // { CardType::PlaceBomb0,   10 },
      // { CardType::PlaceBomb1,    5 },
      // { CardType::PlaceBomb2,    3 },
      // { CardType::SetupJammer,   2 },
      // { CardType::Block,         2 },
      // { CardType::Release,       1 },
      { CardType::FalseAlarm,   3  },
      { CardType::PlaceBomb0,   15 },
      { CardType::PlaceBomb1,   10 },
      { CardType::PlaceBomb2,   5  },
      { CardType::Release,      1  },
    };


    std::vector<CardType> createDeck(int32_t players, gf::ArrayRef<DeckData> config, gf::Random& random) {
      std::vector<CardType> deck;

      for (auto data : config) {
        for (int i = 0; i < data.count * players; ++i) {
          deck.push_back(data.card);
        }
      }

      std::shuffle(deck.begin(), deck.end(), random.getEngine());
      return deck;
    }

  }


  Deck::Deck(int32_t players, gf::Random& random)
  : m_protectorIndex(0)
  , m_protectorsDeck(createDeck(Crew::computeProtectors(players), ProtectorDeckConfig, random))
  , m_rebelIndex(0)
  , m_rebelsDeck(createDeck(Crew::computeRebels(players), RebelDeckConfig, random))
  {
  }

  CardType Deck::pickProtectorCard() {
    assert(m_protectorIndex < m_protectorsDeck.size());
    return m_protectorsDeck[m_protectorIndex++];
  }

  CardType Deck::pickRebelCard() {
    assert(m_rebelIndex < m_rebelsDeck.size());
    return m_rebelsDeck[m_rebelIndex++];
  }

}
