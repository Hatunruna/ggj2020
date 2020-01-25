#include "Scenes.h"

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "Constants.h"

namespace ggj {

  Scenes::Scenes(ClientNetwork& network, gf::Path searchDir)
  : gf::SceneManager("tmpgame", InitialSize)
  , resources({ searchDir })
  , intro(*this, resources)
  , connection(*this, network, resources)
  , lobby(*this, network, resources)
  , room(*this, resources)
  , waiting(*this, resources)
  {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // config
    // io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr;

    // load font(s)
    // io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels);

    ImGui_ImplGF_Init(getWindow(), getRenderer());
  }

  Scenes::~Scenes() {
    ImGui::DestroyContext();
  }

}

