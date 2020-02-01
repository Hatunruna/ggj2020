#include "GameScene.h"

#include <cinttypes>

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
  , m_ship(resources)
  , m_info(resources)
  , m_chat(network, m_players)
  , m_electedPlayers(gf::InvalidId)
  , m_gamePhase(GamePhase::CapitainElection)
  , m_alreadyVote(false)
  , m_showStartMoveAndPlayButton(false)
  , m_startMoveAndPlayButton("Start", resources.getFont("DejaVuSans.ttf"))
  {
    setWorldViewSize(WorldSize);
    setWorldViewCenter(WorldSize * 0.5f);

    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);

    addWorldEntity(m_ship);

    addHudEntity(m_info);
    getWorldView().setViewport(gf::RectF::fromPositionSize({0.0f, 0.0f}, {1.0f, 2.f / 3.f}));

    m_startMoveAndPlayButton.setDefaultBackgroundColor(gf::Color::Gray(0.75f));
    m_startMoveAndPlayButton.setDefault();
  }

  void GameScene::initialize(const std::vector<PlayerData> &players) {
    for(auto &player: players) {
      ClientPlayerData playerData;
      playerData.id = player.id;
      playerData.name = player.name;
      m_players.emplace(player.id, playerData);
    }
    m_electedPlayers = gf::InvalidId;
    m_gamePhase = GamePhase::CapitainElection;
    m_alreadyVote = false;
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

	  if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
      if (m_startMoveAndPlayButton.contains(event.mouseButton.coords)) {
        m_showStartMoveAndPlayButton = false;
        PemClientStartMoveAndPlay message;
        m_network.send(message);
      }

      gf::Vector2f relativeCoords = gf::Vector2f(event.mouseButton.coords) / m_scenes.getRenderer().getSize();
      CardType clickedCardType;
      if (m_info.getCardType(relativeCoords, clickedCardType)) {
        // TODO handle clickedCardType
        gf::Log::debug("Clicked card: %s\n", cardTypeString(clickedCardType).c_str());
      }

      gf::Vector2f worldCoords = m_scenes.getRenderer().mapPixelToCoords(event.mouseButton.coords, getWorldView());
      PlaceType clickedPlaceType;
      if (m_ship.getPlaceType(worldCoords, clickedPlaceType)) {
        // TODO handle clickedPlaceType
        gf::Log::debug("Clicked place: %s\n", placeTypeString(clickedPlaceType).c_str());
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

        case PemServerChooseCaptain::type: {
          gf::Log::debug("[game] receive PemServerChooseCaptain\n");
          auto data = bytes.as<PemServerChooseCaptain>();

          auto it = m_players.find(data.member);
          if (it != m_players.end()) {
            it->second.captain = true;

            MessageData message;
            message.origin = gf::InvalidId;
            message.author = "server";
            message.content = it->second.name + " is the new captain";

            m_chat.appendMessage(std::move(message));
          }

          if (data.member == m_scenes.myPlayerId) {
            m_showStartMoveAndPlayButton = true;
          }
        }

        case PemServerStartMoveAndPlay::type: {
          
        }
      }
    }
  }

  void GameScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coordinates(target);

    // Election window
    gf::Vector2f electionWindowSize = coordinates.getRelativeSize({ 0.25f, 0.28f });
    gf::Vector2f electionWindowPos = coordinates.getCenter();

    ImGui::NewFrame();
    if (m_gamePhase == GamePhase::CapitainElection && !m_alreadyVote) {
      ImGui::SetNextWindowSize(ImVec2(electionWindowSize.width, electionWindowSize.height));
      ImGui::SetNextWindowPos(ImVec2(electionWindowPos.x, electionWindowPos.y), 0, ImVec2(0.5f, 0.5f));

      if (ImGui::Begin("Vote for your Capitain", nullptr, DefaultWindowFlags)) {
        // List players
        unsigned i = 0;
        for (auto &player: m_players) {
          std::string name = std::to_string(i) + ". " + player.second.name;
          if (ImGui::Selectable(name.c_str(), m_electedPlayers == player.second.id)) {
            m_electedPlayers = player.second.id;
          }
          ++i;
        }
        if (ImGui::Selectable("None Of The Above", m_electedPlayers == gf::InvalidId)) {
          m_electedPlayers = gf::InvalidId;
        }

        if (ImGui::Button("Vote", ImVec2(ImGui::GetWindowWidth(), DefaultButtonSize.y))) {
          gf::Log::debug("(GAME) Vote for: %" PRIX64 "\n", m_electedPlayers);

          PemClientVoteForCaptain vote;
          vote.member = m_electedPlayers;
          m_network.send(vote);

          m_alreadyVote = true;
        }
      }
      ImGui::End();
    }

    // Chat window
    m_chat.display(10, coordinates);

    // Default display
    renderWorldEntities(target, states);
    renderHudEntities(target, states);

    //Start move and play button
    if (m_showStartMoveAndPlayButton) {
      m_startMoveAndPlayButton.setCharacterSize(coordinates.getRelativeCharacterSize(0.05f));
      m_startMoveAndPlayButton.setPosition(coordinates.getRelativePoint({0.05f, 0.6f}));

      target.draw(m_startMoveAndPlayButton, states);
    }

    ImGui::Render();
    ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
  }

}
