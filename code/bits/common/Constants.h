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

    constexpr gf::Vector2f WorldSize = { 3960.0f, 1500.0f };
    constexpr gf::Vector2f ViewSize = { 2000.0f, 2000.0f };
    constexpr float MinimalHeight = 1500.0f;
    constexpr float MaximalHeight = 1500.0f;

    constexpr int MaxCards = 3;
    constexpr float BasicMoveSpeed = 1000;
}

#endif // TMPGAME_CONSTANTS_H
