#ifndef TMPGAME_VIEW_ADAPTATOR_H
#define TMPGAME_VIEW_ADAPTATOR_H

#include <gf/RenderTarget.h>
#include <gf/View.h>

namespace ggj {
  class ViewAdaptator {
  public:
    ViewAdaptator(const gf::RenderTarget& target, gf::View& view);

    void processEvent(const gf::Event& event);

  private:
    const gf::RenderTarget& m_target;
    gf::View& m_view;
    gf::Vector2i m_mousePosition;

    enum class State {
      Stationary,
      Moving,
    };

    State m_state;

    float m_zoomLevel;
  };
}

#endif // TMPGAME_VIEW_ADAPTATOR_H
