#ifndef PEM_PEM_INSTANCE_H
#define PEM_PEM_INSTANCE_H

#include <gf/Random.h>

#include "GameInstance.h"
#include "Deck.h"
#include "Member.h"
#include "Ship.h"

namespace pem {

  class PemInstance : public GameInstance {
  public:
    PemInstance(int32_t players);

    void start() override;

    bool isFinished() override;

    void update(ServerPlayer& player, gf::Packet& packet) override;

  private:
    void resetTurn();
    void resetVote();
    void checkEndOfVote();
    void checkEndOfTurn();
    void updateMission();

  private:
    gf::Random m_random;
    int32_t m_players;
    Deck m_deck;
    std::map<gf::Id, Member> m_members;
    Ship m_ship;
    std::map<gf::Id, gf::Id> m_votes;
    float m_distance;
    int m_turn;
  };

}

#endif // PEM_PEM_INSTANCE_H
