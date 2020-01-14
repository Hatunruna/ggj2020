#include "MenuEntity.h"

namespace ggj {

  MenuEntity::MenuEntity(gf::ResourceManager& resources)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_startWidget("Start", m_font)
  , m_quitWidget("Quit", m_font)
  {
    m_container.addWidget(m_startWidget);
    m_container.addWidget(m_quitWidget);
  }

  void MenuEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {

  }


}


