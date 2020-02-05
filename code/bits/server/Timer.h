#ifndef PEM_TIMER_H
#define PEM_TIMER_H

#include <functional>

#include <gf/Time.h>

namespace pem {

  template<typename Signature>
  class Timer {
  public:
    Timer(gf::Time deadline, std::function<Signature> function)
    : m_deadline(deadline)
    , m_function(function) {

    }

  private:
    gf::Time m_deadline;
    gf::Time m_elapsedTime;
    std::function<Signature> m_function;
  };

}

#endif // PEM_TIMER_H
