#include "Scenes.h"

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "common/Constants.h"

namespace pem {

  Scenes::Scenes(gf::Path searchDir)
  : gf::SceneManager("Pax et Mors", InitialSize)
  , resources({ searchDir })
  , myPlayerId(gf::InvalidId)
  , m_asyncLoading(false)
  , m_loadingFinished(false)
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
    ImGui_ImplGF_Shutdown();
    ImGui::DestroyContext();
  }

  void Scenes::loadingAssets(ClientNetwork& network) {
    if (!m_asyncLoading) {
      intro = std::make_unique<IntroScene>(*this, resources);
    }
    connection = std::make_unique<ConnectionScene>(*this, resources, network);
    lobby = std::make_unique<LobbyScene>(*this, resources, network);
    room = std::make_unique<RoomScene>(*this, resources, network);
    game = std::make_unique<GameScene>(*this, network, resources);
    credits = std::make_unique<CreditsScene>(*this, resources);
    help = std::make_unique<HelpScene>(*this, resources);

    m_loadingFinished = true;
  }

  void Scenes::loadingAsynchronousAssets(ClientNetwork& network) {
    m_asyncLoading = true;
    resources.asynchronousLoading(getWindow(), [this, &network](){
      loadingAssets(network);
    });

    // Handle splash screen...
    intro = std::make_unique<IntroScene>(*this, resources);
  }

  bool Scenes::loadingFinished() {
    if (m_asyncLoading) {
      return resources.waitLoading();
    }

    return m_loadingFinished;
  }

}
