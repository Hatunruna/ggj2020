#ifndef PEM_MENU_ENTITY_H
#define PEM_MENU_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

namespace pem {

  enum class MenuChoice {
    None,
    Start,
    Help,
    Credits,
    Quit,
  };

  class MenuEntity : public gf::Entity {
  public:
    MenuEntity(gf::ResourceManager& resources, MenuChoice& choice);

    void pointTo(gf::Vector2f coords);
    void triggerAction();

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font& m_font;
    gf::WidgetContainer m_container;
    gf::TextWidget m_startWidget;
    gf::TextWidget m_helpWidget;
    gf::TextWidget m_creditsWidget;
    gf::TextWidget m_quitWidget;
  };

}

#endif // PEM_MENU_ENTITY_H
