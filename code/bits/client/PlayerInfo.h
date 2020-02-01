#ifndef TMPGAME_PLAYER_INFO_H
#define TMPGAME_PLAYER_INFO_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>

#include "common/PemProtocol.h"

namespace ggj {
  class PlayerInfo: public gf::Entity {
  public:
    PlayerInfo(gf::ResourceManager& resources);

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
	  bool getCardType(const gf::Vector2f& pos, CardType& res) const;

    void initializeHand(const std::array<CardType, MaxCards>& cards);

    void setRole(const std::string &role) {
      m_role = role;
    }

  private:
    gf::RectF getCardBounds(std::size_t i) const;

  private:
    gf::Font &m_font;
    std::string m_role;
    std::array<CardType, MaxCards> m_cards;
  };

}


#endif // TMPGAME_PLAYER_INFO_H
