#include "Scenes.h"

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "common/Constants.h"

namespace pem {

  Scenes::Scenes(ClientNetwork& network, gf::Path searchDir)
  : gf::SceneManager("Pax et Mors", InitialSize)
  , resources({ searchDir })
  , intro(*this, resources)
  , connection(*this, resources, network)
  , lobby(*this, resources, network)
  , room(*this, resources, network)
  , game(*this, network, resources)
  , credits(*this, resources)
  , help(*this, resources)
  , myPlayerId(gf::InvalidId)
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

