#ifndef TMPGAME_CONSTANTS_H
#define TMPGAME_CONSTANTS_H

#include <gf/Vector.h>

namespace ggj {

  constexpr gf::Vector2i InitialSize = { 1024, 768 };
  constexpr unsigned UICharacterSize = 14;
  constexpr float UILineSize = UICharacterSize * 2.0f;

  constexpr std::size_t PlayersCount = 2;
  constexpr const char *Service = "2020";

  constexpr std::size_t MaxPlayerNameLength = 15;
  constexpr std::size_t MaxRoomNameLength = 15;

}

#endif // TMPGAME_CONSTANTS_H
