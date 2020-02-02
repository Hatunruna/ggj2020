#ifndef TMPGAME_SHIP_ENTITY_H
#define TMPGAME_SHIP_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Shapes.h>

#include "common/PemTypes.h"

namespace ggj {
  class ShipEntity: public gf::Entity {
  public:
    ShipEntity(gf::ResourceManager& resources);

    void updateMouseCoords(const gf::Vector2i& coords);
    void selectPlace(PlaceType place);

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
    bool getPlaceType(const gf::Vector2f& position, PlaceType& res) const;

  private:
    gf::Font &m_font;
    gf::Texture &m_shipTexture;
    gf::RectangleShape m_rect;
    gf::Vector2i m_mouseCoords;
    PlaceType m_selectedPlace;
  };
}

#endif // TMPGAME_SHIP_ENTITY_H
