#ifndef TMPGAME_CONSTANTS_H
#define TMPGAME_CONSTANTS_H

#include <gf/Vector.h>

namespace ggj {

  constexpr gf::Vector2i InitialSize = { 1024, 768 };

  constexpr std::size_t PlayersCount = 2;
  constexpr const char *Service = "2020";

  constexpr std::size_t MaxPlayerNameLength = 64;

}

#endif // TMPGAME_CONSTANTS_H
