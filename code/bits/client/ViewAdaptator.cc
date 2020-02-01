#include "ViewAdaptator.h"

#include <gf/Event.h>
#include <gf/Log.h>

#include "common/Constants.h"

namespace ggj {
  ViewAdaptator::ViewAdaptator(const gf::RenderTarget& target, gf::View& view)
  : m_target(target)
  , m_view(view)
  , m_mousePosition({ 0, 0 })
  , m_state(State::Stationary)
  , m_zoomLevel(1.0f)
  {

  }

  namespace {

    bool isCursorOnView(gf::Vector2i cursor, gf::Vector2i screenSize, const gf::RectF& viewport) {
      gf::RectF visible = gf::RectF::fromPositionSize(viewport.getPosition() * screenSize, viewport.getSize() * screenSize);
      return visible.contains(cursor);
    }

  }

  void ViewAdaptator::processEvent(const gf::Event& event) {
    static constexpr float ZoomInFactor = 0.8f;
    static constexpr float ZoomOutFactor = 1.25f;
    static constexpr float ZoomMax = WorldSize.y / MinimalHeight;
    static constexpr float ZoomMin = WorldSize.y / MaximalHeight;

    // gf::Log::debug("max: %f min %f\n", ZoomMax, ZoomMin);

    switch (event.type) {
      case gf::EventType::MouseMoved:
        if (m_state == State::Moving) {
          gf::Vector2f oldPosition = m_target.mapPixelToCoords(m_mousePosition, m_view);
          gf::Vector2f newPosition = m_target.mapPixelToCoords(event.mouseCursor.coords, m_view);

          gf::Vector2f move = oldPosition - newPosition;
          move.y = 0.0f;

          // gf::clamp(move.x, m_bounds.getTopLeft().x, m_bounds.getTopRight().x);

          m_view.move(move);
        }

        m_mousePosition = event.mouseCursor.coords;
        break;

      case gf::EventType::MouseButtonPressed:
        if (isCursorOnView(event.mouseButton.coords, m_target.getSize(), m_view.getViewport())) {
          m_state = State::Moving;
        }
        break;

      case gf::EventType::MouseButtonReleased:
        m_state = State::Stationary;
        break;

      case gf::EventType::MouseWheelScrolled: {
        auto center = m_view.getCenter();
        auto fixed = m_target.mapPixelToCoords(m_mousePosition, m_view);
        fixed.y = center.y;


        if (event.mouseWheel.offset.y > 0) {
          m_view.zoom(ZoomInFactor, fixed);
        } else {
          m_view.zoom(ZoomOutFactor, fixed);
        }

        // if (event.mouseWheel.offset.y > 0 && m_zoomLevel > ZoomMin) {
        //   m_zoomLevel *= ZoomInFactor;
        //   m_zoomLevel = gf::clamp(m_zoomLevel, ZoomMin, ZoomMax);
        //   m_view.zoom(m_zoomLevel, fixed);
        // } else {
        //   m_zoomLevel *= ZoomOutFactor;
        //   m_zoomLevel = gf::clamp(m_zoomLevel, ZoomMin, ZoomMax);
        //   m_view.zoom(m_zoomLevel, fixed);
        // }

        gf::Log::debug("current zoom %f\n", m_zoomLevel);
        break;
      }

      default:
        break;
    }
  }
}
