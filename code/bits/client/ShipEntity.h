#ifndef TMPGAME_SHIP_ENTITY_H
#define TMPGAME_SHIP_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Shapes.h>

#include <gf/Animation.h>
#include "common/PemTypes.h"

namespace ggj {
  class ShipEntity: public gf::Entity {
  public:
    ShipEntity(gf::ResourceManager& resources);

    PlaceType getPlaceType(const gf::Vector2f& position) const;
    void updateMouseCoords(const gf::Vector2i& coords);
    void selectPlace(PlaceType place);
    void setPlaceState(PlaceType place, bool state);
    void stopDrawWarnings();

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font &m_font;
    gf::Texture &m_shipTexture;
    gf::Texture &m_engineTexture;
    gf::Animation m_engineAnimation;
    gf::RectangleShape m_rect;
    gf::Vector2i m_mouseCoords;
    PlaceType m_selectedPlace;
    bool m_drawWarning;
  };
}

#endif // TMPGAME_SHIP_ENTITY_H
