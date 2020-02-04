#ifndef TMPGAME_PLAYER_INFO_H
#define TMPGAME_PLAYER_INFO_H

#include <gf/TextureAtlas.h>
#include <gf/Entity.h>
#include <gf/ResourceManager.h>

#include "common/PemProtocol.h"

namespace ggj {
  class PlayerInfo: public gf::Entity {
  public:
    PlayerInfo(gf::ResourceManager& resources);

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
	  CardType getCardType(const gf::Vector2f& screenPosition, const gf::Vector2f& screenSize);

    void showCards(bool show = true) {
      m_showCards = show;
    }

    void resetCardSelection() {
      m_selectedCard = -1;
    }

    void replaceCard(CardType type);

    void initializeHand(const std::array<CardType, MaxCards>& cards);

    void setRole(CrewType role) {
      m_role = role;
    }

  private:
    gf::RectF getCardBounds(gf::Vector2f screeSize, std::size_t i) const;

  private:
    gf::Font &m_font;
    gf::Texture &m_emptyCardTexture;
    gf::TextureAtlas m_atlas;
    CrewType m_role;
    std::array<CardType, MaxCards> m_cards;
    int m_selectedCard;
    bool m_showCards;
  };

}


#endif // TMPGAME_PLAYER_INFO_H
