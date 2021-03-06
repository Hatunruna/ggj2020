#include "ViewAdaptator.h"

#include <gf/Event.h>
#include <gf/Log.h>

#include "common/Constants.h"

namespace pem {
  ViewAdaptator::ViewAdaptator(const gf::RenderTarget& target, gf::View& view)
  : m_target(target)
  , m_view(view)
  , m_mousePosition({ 0, 0 })
  , m_state(State::Stationary)
  , m_zoomLevel(0)
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
    static constexpr int ZoomMax = 1;
    static constexpr int ZoomMin = -4;

    switch (event.type) {
      case gf::EventType::MouseMoved:
        if (m_state == State::Moving) {
          gf::Vector2f oldPosition = m_target.mapPixelToCoords(m_mousePosition, m_view);
          gf::Vector2f newPosition = m_target.mapPixelToCoords(event.mouseCursor.coords, m_view);

          gf::Vector2f move = oldPosition - newPosition;

          auto center = m_view.getCenter();

          if (center.x + move.x < 0.f) {
            move.x = -center.x;
          }
          if (center.x + move.x > WorldSize.x) {
            move.x = WorldSize.x - center.x;
          }

          if (center.y + move.y < 0.f) {
            move.y = -center.y;
          }
          if (center.y + move.y > WorldSize.y) {
            move.y = WorldSize.y - center.y;
          }

          m_view.move(move);
        }

        m_mousePosition = event.mouseCursor.coords;
        break;

      case gf::EventType::MouseButtonPressed:
        if (isCursorOnView(event.mouseButton.coords, m_target.getSize(), m_view.getViewport()) && event.mouseButton.button == gf::MouseButton::Right) {
          m_state = State::Moving;
        }
        break;

      case gf::EventType::MouseButtonReleased:
        if (event.mouseButton.button == gf::MouseButton::Right) {
          m_state = State::Stationary;
        }
        break;

      case gf::EventType::MouseWheelScrolled: {
        auto fixed = m_target.mapPixelToCoords(m_mousePosition, m_view);

        if (event.mouseWheel.offset.y > 0) {
          if (m_zoomLevel > ZoomMin) {
            --m_zoomLevel;
            m_view.zoom(ZoomInFactor, fixed);
          }
        } else {
          if (m_zoomLevel < ZoomMax) {
            ++m_zoomLevel;
            m_view.zoom(ZoomOutFactor, fixed);
          }
        }

        break;
      }

      default:
        break;
    }
  }
}
