#ifndef TMPGAME_PEM_INSTANCE_H
#define TMPGAME_PEM_INSTANCE_H

#include <gf/Random.h>

#include "GameInstance.h"
#include "Deck.h"
#include "Member.h"

namespace ggj {

  class PemInstance : public GameInstance {
  public:
    PemInstance(int32_t players);

    void start() override;

    bool isFinished() override;

    void update(ServerPlayer& player, ProtocolBytes& bytes) override;

  private:
    void checkEndOfVote();

  private:
    gf::Random m_random;
    int32_t m_players;
    Deck m_deck;
    std::map<gf::Id, Member> m_members;
    std::map<gf::Id, int32_t> m_votes;
  };

}

#endif // TMPGAME_PEM_INSTANCE_H
