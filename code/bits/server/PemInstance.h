#ifndef TMPGAME_PEM_INSTANCE_H
#define TMPGAME_PEM_INSTANCE_H

#include <gf/Random.h>

#include "GameInstance.h"
#include "Deck.h"
#include "Member.h"
#include "Ship.h"

namespace ggj {

  class PemInstance : public GameInstance {
  public:
    PemInstance(int32_t players);

    void start() override;

    bool isFinished() override;

    void update(ServerPlayer& player, ProtocolBytes& bytes) override;

    void resolution();

    void execute(PlaceType place,CardType type);

  private:
    void checkEndOfVote(VoteType type);
    void checkEndOfTurn();
    bool isPlayable(PlaceState state,CardType type);

  private:
    gf::Random m_random;
    int32_t m_players;
    int32_t m_currentlyPlaying;
    Deck m_deck;
    std::map<gf::Id, Member> m_members;
    std::map<gf::Id, int32_t> m_votes;
    std::map<PlaceType, std::map<CardType,int>> m_action ;
    Ship m_ship;
  };

}

#endif // TMPGAME_PEM_INSTANCE_H
