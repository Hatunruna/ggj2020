#ifndef PEM_CONSTANTS_H
#define PEM_CONSTANTS_H

#include <gf/Vector.h>

namespace pem {

    constexpr gf::Vector2i InitialSize = { 1280, 720 };
    constexpr unsigned UICharacterSize = 14;
    constexpr float UILineSize = UICharacterSize * 2.0f;

    constexpr std::size_t PlayersCount = 2;
    constexpr const char *Service = "8000";

    constexpr std::size_t MaxPlayerNameLength = 15;
    constexpr std::size_t MaxRoomNameLength = 15;

    constexpr gf::Vector2f WorldSize = { 2953.0f, 2045.0f }; // It's the background size
    constexpr gf::Vector2f ViewSize = { 1500.0f, 1500.0f };
    constexpr float MinimalHeight = 1500.0f;
    constexpr float MaximalHeight = 1500.0f;

    constexpr int MaxCards = 3;
    constexpr float BasicMoveSpeed = 1000;

    constexpr int MaxSentence = 2;
}

#endif // PEM_CONSTANTS_H
