#include "Scenes.h"

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "common/Constants.h"

namespace ggj {

  Scenes::Scenes(ClientNetwork& network, gf::Path searchDir)
  : gf::SceneManager("tmpgame", InitialSize)
  , resources({ searchDir })
  , intro(*this, resources)
  , connection(*this, network)
  , lobby(*this, network)
  , room(*this, network)
  , waiting(*this, resources)
  {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // config
    // io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr;

    // load font(s)
    io.Fonts->AddFontFromFileTTF(resources.getAbsolutePath("DejaVuSans.ttf").string().c_str(), 20);

    ImGui_ImplGF_Init(getWindow(), getRenderer());
  }

  Scenes::~Scenes() {
    ImGui::DestroyContext();
  }

}

