#ifndef PEM_SHIP_ENTITY_H
#define PEM_SHIP_ENTITY_H

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Shapes.h>

#include "common/PemTypes.h"

#include "GameModel.h"

namespace pem {
  class ShipEntity: public gf::Entity {
  public:
    ShipEntity(gf::ResourceManager& resources, GameModel &model);

    void updateMouseCoords(const gf::Vector2f& coords);
    void updateSelectedPlace();

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font &m_font;

    gf::Texture &m_shipTexture;
    gf::Texture &m_engineTexture;

    gf::Animation m_engineAnimation;

    gf::Vector2f m_mouseCoords;

    GameModel &m_model;
  };
}

#endif // PEM_SHIP_ENTITY_H
