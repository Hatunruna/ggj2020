#ifndef PEM_SHIP_ENTITY_H
#define PEM_SHIP_ENTITY_H

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/EntityContainer.h>
#include <gf/ResourceManager.h>
#include <gf/Shapes.h>

#include "common/PemTypes.h"

#include "GameModel.h"


namespace pem {

  class PlaceEntity: public gf::Entity {
  public:
    PlaceEntity(gf::Texture &workingTexture,
      std::vector<gf::Ref<gf::Texture>> brokenTextures,
      GameModel &model, PlaceType place,
      int lastNumberFrame = 24, gf::Vector2i tilesetLayout = { 4, 6});

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    pem::GameModel &m_model;

    PlaceType m_place;
    gf::Texture &m_workingTexture;
    gf::Animation m_brokenAnimation;
    gf::Vector2f m_position;
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

    // Container for all other animations
    std::vector<PlaceEntity> m_places;

    gf::Vector2f m_mouseCoords;

    GameModel &m_model;
  };
}

#endif // PEM_SHIP_ENTITY_H
