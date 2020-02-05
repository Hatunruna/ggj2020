#ifndef PEM_VIEW_ADAPTATOR_H
#define PEM_VIEW_ADAPTATOR_H

#include <gf/RenderTarget.h>
#include <gf/View.h>

namespace pem {
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
    int m_zoomLevel;
  };
}

#endif // PEM_VIEW_ADAPTATOR_H
