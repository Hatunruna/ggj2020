#ifndef PEM_SHIP_ENTITY_H
#define PEM_SHIP_ENTITY_H

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Shapes.h>

#include "common/PemTypes.h"

#include "GameModel.h"


namespace pem {

  class PlaceEntity: public gf::Entity {
  public:
    PlaceEntity();

  private:
    gf::Animation m_workingAnimation;
    gf::Animation m_brokenAnimation;
  };

  class ShipEntity: public gf::Entity {
  public:
    ShipEntity(gf::ResourceManager& resources, GameModel &model);

    void updateMouseCoords(const gf::Vector2f& coords);
    void updateSelectedPlace();

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font &m_font;

    // Global ship texture
    gf::Texture &m_shipTexture;

    // Texture and animation for left flame
    gf::Animation m_leftFlameWorkingAnimation;
    gf::Animation m_leftFlameBrokenAnimation;
    gf::Animation m_rightFlameWorkingAnimation;
    gf::Animation m_rightFlameBrokenAnimation;

    gf::Vector2f m_mouseCoords;

    GameModel &m_model;
  };
}

#endif // PEM_SHIP_ENTITY_H
