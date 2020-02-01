#ifndef TMPGAME_SHIP_ENTITY_H
#define TMPGAME_SHIP_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Shapes.h>

namespace ggj {
  class ShipEntity: public gf::Entity {
  public:
    ShipEntity(gf::ResourceManager& resources);

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font &m_font;
    gf::RectangleShape m_rect;
  };
}

#endif // TMPGAME_SHIP_ENTITY_H
