#ifndef PEM_PLAYER_INFO_H
#define PEM_PLAYER_INFO_H

#include <gf/TextureAtlas.h>
#include <gf/Entity.h>
#include <gf/ResourceManager.h>

#include "common/PemProtocol.h"

#include "GameModel.h"

namespace pem {
  class PlayerInfo: public gf::Entity {
  public:
    PlayerInfo(gf::ResourceManager& resources, GameModel &model);

    void updateSelectedCard(const gf::Vector2f &mouseCoords, const gf::Vector2f& screenSize);

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::RectF getCardBounds(gf::Vector2f screeSize, std::size_t i) const;

  private:
    gf::Font &m_font;
    gf::TextureAtlas m_atlas;

    GameModel &m_model;
  };

}


#endif // PEM_PLAYER_INFO_H
