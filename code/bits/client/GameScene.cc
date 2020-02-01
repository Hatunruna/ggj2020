#include "GameScene.h"

#include <gf/Log.h>
#include <gf/Shapes.h>
#include <gf/Coordinates.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

#include "common/Constants.h"
#include "common/PemProtocol.h"
#include "common/Protocol.h"

#include "common/ImGuiConstants.h"

#include "Scenes.h"
#include "Singletons.h"

namespace ggj {

  GameScene::GameScene(Scenes& scenes, ClientNetwork& network, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_network(network)
  , m_scenes(scenes)
  , m_escapeAction("Escape")
  , m_adaptator(m_scenes.getRenderer(), getWorldView())
  , m_info(resources)
  , m_chat(network)
  {
    setWorldViewSize({2000.0f, 1000.0f});
    setWorldViewCenter({ 1000.0f, 500.0f});

    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);

    addWorldEntity(m_ship);

    addHudEntity(m_info);
    getWorldView().setViewport(gf::RectF::fromPositionSize({0.0f, 0.0f}, {1.0f, 2.f / 3.f}));
  }

  void GameScene::doHandleActions(gf::Window& window) {
    if (m_escapeAction.isActive()) {
      m_scenes.setClearColor(gf::Color::White);
      m_scenes.replaceScene(m_scenes.intro);
    }
  }

  bool GameScene::doEarlyProcessEvent(gf::Event &event) {
    return ImGui_ImplGF_ProcessEvent(event);
  }

  void GameScene::doProcessEvent(gf::Event &event) {
    m_adaptator.processEvent(event);

	if (event.type == gf::EventType::MouseButtonPressed) {
		gf::Vector2f relativeCoords = gf::Vector2f(event.mouseButton.coords) / m_scenes.getRenderer().getSize();
		CardType clickedCardType;
		if (m_info.getCardType(relativeCoords, clickedCardType)) {
			// TODO handle clickedCardType
			gf::Log::debug("%hu\n", static_cast<std::uint16_t>(clickedCardType));
		}
	}
  }

  void GameScene::doUpdate(gf::Time time) {
    ImGui_ImplGF_Update(time);

    ProtocolBytes bytes;

    //ggj::gBackgroundMusic.stop();

    while (m_network.queue.poll(bytes)) {
      switch (bytes.getType()) {
        case PemServerInitRole::type: {
          gf::Log::debug("[game] receive PemServerInitRole\n");
          auto data = bytes.as<PemServerInitRole>();
          m_info.setRole(crewTypeString(data.role));
          m_info.initializeHand(data.cards);
          break;
        }

        case ServerChatMessage::type: {
          gf::Log::debug("[game] receive ServerChatMessage\n");
          auto data = bytes.as<ServerChatMessage>();
          m_chat.appendMessage(std::move(data.message));
          break;
        }

        case ServerError::type: {
          auto data = bytes.as<ServerError>();
          MessageData message;
          message.origin = gf::InvalidId;
          message.author = "server";
          message.content = serverErrorString(data.reason);

          m_chat.appendMessage(std::move(message));
          break;
        }
      }
    }
  }

  void GameScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coordinates(target);

    gf::Vector2f chatWindowSize = coordinates.getRelativeSize({ 0.25f, 0.42f });
    gf::Vector2f chatWindowPos = coordinates.getRelativePoint({ 0.80f, 0.78f });

    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(chatWindowSize[0], chatWindowSize[1]));
    ImGui::SetNextWindowPos(ImVec2(chatWindowPos[0], chatWindowPos[1]), 0, ImVec2(0.5f, 0.5f));

    if (ImGui::Begin("Chat", nullptr, DefaultWindowFlags | ImGuiWindowFlags_NoTitleBar))
    {
      m_chat.display(10);
    }
    ImGui::End();


    // Default display
    renderWorldEntities(target, states);
    renderHudEntities(target, states);

    ImGui::Render();
    ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
  }

}
