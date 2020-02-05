#ifndef PEM_GAME_SETTINGS_H
#define PEM_GAME_SETTINGS_H

#include <cstdint>

namespace pem {

  struct GameSettings {
    int32_t teamsMin;
    int32_t teamsMax;
    int32_t playersByTeamMin;
    int32_t playersByTeamMax;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, GameSettings& settings) {
    return ar | settings.teamsMin | settings.teamsMax | settings.playersByTeamMin | settings.playersByTeamMax;
  }

  struct GameInstanceSettings {
    int32_t teams;
    int32_t playersByTeam;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, GameInstanceSettings& settings) {
    return ar | settings.teams | settings.playersByTeam;
  }

}

#endif // PEM_GAME_SETTINGS_H
