#ifndef TMPGAME_IMGUI_CONSTANTS_H
#define TMPGAME_IMGUI_CONSTANTS_H

#include <imgui.h>

namespace ggj {

  constexpr ImGuiWindowFlags DefaultWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
  const ImVec2 DefaultButtonSize = { 170.0f, 40.0f };

}

#endif // TMPGAME_IMGUI_CONSTANTS_H
