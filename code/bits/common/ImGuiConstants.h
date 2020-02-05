#ifndef PEM_IMGUI_CONSTANTS_H
#define PEM_IMGUI_CONSTANTS_H

#include <imgui.h>

namespace pem {

  constexpr ImGuiWindowFlags DefaultWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
  const ImVec2 DefaultButtonSize = { 170.0f, 40.0f };
  const ImVec2 DefaultProgressSize = { 0.0f, 40.0f };

}

#endif // PEM_IMGUI_CONSTANTS_H
