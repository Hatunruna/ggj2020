#ifndef TMPGAME_MENU_ENTITY_H
#define TMPGAME_MENU_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

namespace ggj {

  class MenuEntity : public gf::Entity {
  public:
    MenuEntity(gf::ResourceManager& resources);

    void pointTo(gf::Vector2f coords);
    void triggerAction();

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font& m_font;
    gf::WidgetContainer m_container;
    gf::TextWidget m_startWidget;
    gf::TextWidget m_quitWidget;
  };

}

#endif // TMPGAME_MENU_ENTITY_H
