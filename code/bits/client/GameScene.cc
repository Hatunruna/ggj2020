#include "GameScene.h"

#include <gf/Log.h>
#include <gf/Shapes.h>

#include "common/Constants.h"
#include "common/PemProtocol.h"
#include "common/Protocol.h"

#include "Scenes.h"

namespace ggj {

  GameScene::GameScene(Scenes& scenes, ClientNetwork& network, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_network(network)
  , m_scenes(scenes)
  , m_escapeAction("Escape")
  , m_adaptator(m_scenes.getRenderer(), getWorldView())
  , m_info(resources)
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

  void GameScene::doProcessEvent(gf::Event &event) {
    m_adaptator.processEvent(event);
  }

  void GameScene::doUpdate(gf::Time time) {
    // ImGui_ImplGF_Update(time);

    ProtocolBytes bytes;

    while (m_network.queue.poll(bytes)) {
      switch (bytes.getType()) {
        case PemServerInitRole::type: {
          gf::Log::debug("[game] receive PemServerInitRole\n");
          auto data = bytes.as<PemServerInitRole>();
          m_info.setRole(crewTypeString(data.role));
          m_info.initializeHand(data.cards);
          break;
        }
        // case ServerChatMessage::type: {
        //   auto data = bytes.as<ServerChatMessage>();
        //   m_chat.appendMessage(std::move(data.message));
        //   break;
        // }

        // case ServerError::type: {
        //   auto data = bytes.as<ServerError>();
        //   MessageData message;
        //   message.origin = gf::InvalidId;
        //   message.author = "server";
        //   message.content = serverErrorString(data.reason);

        //   m_chat.appendMessage(std::move(message));
        //   break;
        // }
      }
    }
  }

  void GameScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
    // Default display
    renderWorldEntities(target, states);
    renderHudEntities(target, states);
  }

}
