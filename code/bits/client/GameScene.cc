#include "GameScene.h"

#include <cinttypes>

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Shapes.h>
#include <gf/Unused.h>

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
  , m_scenes(scenes)
  , m_network(network)
  , m_escapeAction("Escape")
  , m_adaptator(m_scenes.getRenderer(), getWorldView())
  , m_ship(resources)
  , m_stars(gf::RectF::fromMinMax({-3000.0f, -1000.0f}, {6000.0f, 3000.0f}))
  , m_info(resources)
  , m_chat(network, m_players)
  , m_gamePhase(GamePhase::CapitainElection)
  , m_votedPlayer(gf::InvalidId)
  , m_alreadyVote(false)
  , m_startMoveAndPlayButton("Continue", resources.getFont("DejaVuSans.ttf"))
  , m_alreadyUsedMoveAndPlayButton(false)
  , m_placeTypeSelected(PlaceType::None)
  {
    setWorldViewSize(WorldSize);
    setWorldViewCenter(WorldSize * 0.5f);

    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);

    addWorldEntity(m_ship);
    addWorldEntity(m_stars);

    addHudEntity(m_info);
    getWorldView().setViewport(gf::RectF::fromPositionSize({0.0f, 0.0f}, {1.0f, 2.f / 3.f}));

    m_startMoveAndPlayButton.setAnchor(gf::Anchor::Center);
    m_startMoveAndPlayButton.setDefaultBackgroundColor(gf::Color::Transparent);
    m_startMoveAndPlayButton.setDefaultTextColor(gf::Color::White);
    m_startMoveAndPlayButton.setDefaultTextOutlineColor(gf::Color::Black);
    m_startMoveAndPlayButton.setSelectedBackgroundColor(gf::Color::Transparent);
    m_startMoveAndPlayButton.setSelectedTextColor(gf::Color::Gray());
    m_startMoveAndPlayButton.setSelectedTextOutlineColor(gf::Color::Black);
    m_startMoveAndPlayButton.setDefault();

    m_ambiantBackground.setBuffer(gResourceManager().getSound("audio/ambiant.ogg"));
    m_ambiantBackground.setVolume(BackgroundAmbiantVolume);
    m_ambiantBackground.setLoop(true);
    m_cardShuffle.setBuffer(gResourceManager().getSound("audio/shuffle_card.ogg"));
    m_cardShuffle.setVolume(FxsVolume);
  }

  void GameScene::initialize(const std::vector<PlayerData> &players) {
    for(auto &player: players) {
      ClientPlayerData playerData;
      playerData.id = player.id;
      playerData.name = player.name;
      m_players.emplace(player.id, playerData);
    }
    m_votedPlayer = gf::InvalidId;
    m_gamePhase = GamePhase::CapitainElection;
    m_alreadyVote = false;
    gBackgroundMusic.stop();
    m_ambiantBackground.play();
    m_cardShuffle.play();
  }

  void GameScene::doHandleActions(gf::Window& window) {
    gf::unused(window);

    if (m_escapeAction.isActive()) {
      m_ambiantBackground.stop();
      gBackgroundMusic.play();
      m_scenes.transitionToScene(m_scenes.intro, 0.4f, m_scenes.fadeEffect);
    }
  }

  bool GameScene::doEarlyProcessEvent(gf::Event &event) {
    return ImGui_ImplGF_ProcessEvent(event);
  }

  void GameScene::doProcessEvent(gf::Event &event) {
    m_adaptator.processEvent(event);

    if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
      if (m_startMoveAndPlayButton.contains(event.mouseButton.coords) && m_gamePhase == GamePhase::CapitainElection && m_players[m_scenes.myPlayerId].captain && !m_alreadyUsedMoveAndPlayButton) {
        gf::Log::debug("(GAME) Click continue\n");
        PemClientStartMoveAndPlay message;
        m_network.send(message);
        m_alreadyUsedMoveAndPlayButton = true;
      }

      gf::Vector2f coords = gf::Vector2f(event.mouseButton.coords);
      CardType clickedCardType;
      if (m_gamePhase == GamePhase::Action && m_placeTypeSelected != PlaceType::None && m_info.getCardType(coords, m_scenes.getRenderer().getSize(), clickedCardType)) {
        //Can select only release type card when in jail
        if (m_players[m_scenes.myPlayerId].jail && clickedCardType != CardType::Release) {
          MessageData message;
          message.origin = gf::InvalidId;
          message.author = "server";
          message.content = "You can only select Release type card when you are in jail!";

          m_chat.appendMessage(std::move(message));
          return;
        }

        // TODO handle clickedCardType
        gf::Log::debug("(GAME) Clicked card: %s\n", cardTypeString(clickedCardType).c_str());
        PemClientMoveAndPlay moveAndPlay;
        moveAndPlay.place = m_placeTypeSelected;
        moveAndPlay.card = clickedCardType;

        gf::Log::debug("(GAME) Action is: %s %s\n", placeTypeString(m_placeTypeSelected).c_str(), cardTypeString(clickedCardType).c_str());
        m_network.send(moveAndPlay);

        m_gamePhase = GamePhase::Resolution;
        m_placeTypeSelected = PlaceType::None;

        bool playFx = true;
        switch (clickedCardType)
        {
        case CardType::Block:
        {
          m_fx.setBuffer(gResourceManager().getSound("audio/block.ogg"));
        }
          break;
        case CardType::Demine:
        {
          m_fx.setBuffer(gResourceManager().getSound("audio/demine.ogg"));
        }
          break;
        case CardType::FalseAlarm:
        {
          m_fx.setBuffer(gResourceManager().getSound("audio/alarm.ogg"));
        }
          break;
        case CardType::PlaceBomb0:
        case CardType::PlaceBomb1:
        case CardType::PlaceBomb2:
        {
          m_fx.setBuffer(gResourceManager().getSound("audio/bomb_armed.ogg"));
        }
          break;
        case CardType::Reinforce1:
        case CardType::Reinforce2:
        {
          m_fx.setBuffer(gResourceManager().getSound("audio/reinforce.ogg"));
        }
          break;
        case CardType::Release:
        {
          m_fx.setBuffer(gResourceManager().getSound("audio/gate.ogg"));
        }
          break;
        case CardType::Repair:
        case CardType::FalseRepair1:
        case CardType::FalseRepair2:
        {
          m_fx.setBuffer(gResourceManager().getSound("audio/repair.ogg"));
        }
          break;
        case CardType::SetupJammer:
        {
          m_fx.setBuffer(gResourceManager().getSound("audio/jammer.ogg"));
        }
          break;
        default:
          playFx = false;
          break;
        }
        if (playFx)
        {
          m_fx.play();
        }
      }

      gf::Vector2f worldCoords = m_scenes.getRenderer().mapPixelToCoords(event.mouseButton.coords, getWorldView());
      // std::printf("      polygon.addPoint({ %gf, %gf });\n", worldCoords.x, worldCoords.y);
      PlaceType clickedPlaceType;
      if (m_gamePhase == GamePhase::Action && !(m_players[m_scenes.myPlayerId].jail) && m_ship.getPlaceType(worldCoords, clickedPlaceType)) {
        // TODO handle clickedPlaceType
        gf::Log::debug("(GAME) Clicked place: %s\n", placeTypeString(clickedPlaceType).c_str());
        m_placeTypeSelected = clickedPlaceType;
        m_ship.selectPlace(clickedPlaceType);

        m_fx.setBuffer(gResourceManager().getSound("audio/foot_steps.ogg"));
        m_fx.setVolume(FxsVolume);
        m_fx.play();
      }
	  }
    else if (event.type == gf::EventType::MouseMoved) {
      // If we are playing
      if (m_gamePhase == GamePhase::Action && !(m_players[m_scenes.myPlayerId].jail)) {
        gf::Vector2f worldCoords = m_scenes.getRenderer().mapPixelToCoords(event.mouseCursor.coords, getWorldView());
        m_ship.updateMouseCoords(worldCoords);
      }

      if(m_startMoveAndPlayButton.contains(event.mouseCursor.coords)) {
        m_startMoveAndPlayButton.setSelected();
      } else {
        m_startMoveAndPlayButton.setDefault();
      }
    }

  }

  void GameScene::doUpdate(gf::Time time) {
    if (!isActive()) {
      return;
    }

    ImGui_ImplGF_Update(time);

    ProtocolBytes bytes;

    while (m_network.queue.poll(bytes)) {
      switch (bytes.getType()) {
        case PemServerInitRole::type: {
          gf::Log::debug("[game] receive PemServerInitRole\n");
          auto data = bytes.as<PemServerInitRole>();
          m_info.setRole(data.role);
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
          break;
        }

        case PemServerStartMoveAndPlay::type: {
          gf::Log::debug("[game] receive PemServerStartMoveAndPlay\n");
          MessageData message;
          message.origin = gf::InvalidId;
          message.author = "server";
          message.content = "It's your turn to play";
          m_chat.appendMessage(std::move(message));
          m_ship.stopDrawWarnings();

          m_gamePhase = GamePhase::Action;
          m_alreadyUsedMoveAndPlayButton = false;

          if (m_players[m_scenes.myPlayerId].jail) {
            m_placeTypeSelected = PlaceType::Prison;
          }
          break;
        }

        case PemServerStartVoteForPrisoner::type: {
          gf::Log::debug("[game] receive PemServerStartVoteForPrisoner\n");

          m_alreadyVote = (m_players[m_scenes.myPlayerId].jail);
          m_ship.selectPlace(PlaceType::None);
          m_info.resetCardSelection();
          m_gamePhase = GamePhase::Meeting;
          break;
        }

        case PemServerChoosePrisoner::type: {
          gf::Log::debug("[game] receive PemServerChoosePrisoner\n");
          auto data = bytes.as<PemServerChoosePrisoner>();

          MessageData message;
          message.origin = gf::InvalidId;
          message.author = "server";

          auto it = m_players.find(data.member);
          if (it != m_players.end()) {
            it->second.jail = true;
            message.content = it->second.name + " is now jailed";
          } else {
            message.content = "No one has been in prison";
          }
          m_chat.appendMessage(std::move(message));

          m_gamePhase = GamePhase::CapitainElection;
          m_alreadyVote = true;
          break;
        }

        case PemServerUpdateShip::type: {
          gf::Log::debug("[game] receive PemServerUpdateShip\n");

          auto data = bytes.as<PemServerUpdateShip>();
          for (auto &entry: data.state) {
            m_ship.setPlaceState(entry.first, entry.second);
          }

          break;
        }

        case PemServerResolution::type: {
          gf::Log::debug("[game] receive PemServerResolution\n");
          auto data = bytes.as<PemServerResolution>();

          for(auto &resolution: data.conclusion) {
            MessageData message;
            message.origin = gf::InvalidId;
            message.author = "server";

            switch (resolution.type) {
              case ResolutionType::Examine: {
                if (resolution.bomb) {
                  message.content = "There is a bomb.";
                } else {
                  message.content = "There is no bomb.";
                }
                break;
              }
              case ResolutionType::Hide: {
                if (resolution.members.size() > 0) {
                  message.content = "I saw ";
                  for (auto &playerID: resolution.members) {
                    message.content += m_players.at(playerID).name + " ";
                  }
                  message.content += " hiding.";
                } else {
                  message.content = "I saw nobody.";
                }
                break;
              }
              case ResolutionType::Track: {
                if (resolution.members.size() > 0) {
                  message.content = "I detected ";
                  for (auto &playerID: resolution.members) {
                    message.content += m_players.at(playerID).name + " ";
                  }
                } else {
                  message.content = "I detected nobody.";
                }
                break;
              }
              case ResolutionType::Block: {
                message.content = "The room was blocked.";
                break;
              }
              case ResolutionType::Release: {
                for (auto &playerID: resolution.members) {
                  m_players[playerID].jail = false;
                  message.content += m_players[playerID].name + " ";
                }
                message.content += "are released from prison.";
                break;
              }
            }

            m_chat.appendMessage(std::move(message));
          }

          break;
        }

        case PemServerUpdateHand::type: {
          gf::Log::debug("[game] receive PemServerUpdateHand\n");

          auto data = bytes.as<PemServerUpdateHand>();
          m_info.replaceCard(data.card);

          break;
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
          if (ImGui::Selectable(name.c_str(), m_votedPlayer == player.second.id)) {
            m_votedPlayer = player.second.id;
          }
          ++i;
        }
        if (ImGui::Selectable("None Of The Above", m_votedPlayer == gf::InvalidId)) {
          m_votedPlayer = gf::InvalidId;
        }

        if (ImGui::Button("Vote", ImVec2(ImGui::GetWindowWidth(), DefaultButtonSize.y))) {
          gf::Log::debug("(GAME) Vote for: %" PRIX64 "\n", m_votedPlayer);

          PemClientVoteForCaptain vote;
          vote.member = m_votedPlayer;
          m_network.send(vote);

          m_alreadyVote = true;
        }
      }
      ImGui::End();
    }

    //Prisoner vote window
    if (m_gamePhase == GamePhase::Meeting && !m_alreadyVote) {
      ImGui::SetNextWindowSize(ImVec2(electionWindowSize.width, electionWindowSize.height));
      ImGui::SetNextWindowPos(ImVec2(electionWindowPos.x, electionWindowPos.y), 0, ImVec2(0.5f, 0.5f));

      if (ImGui::Begin("Vote for the next prisoner", nullptr, DefaultWindowFlags)) {
        // List players
        unsigned i = 0;
        for (auto &player: m_players) {
          if (!(player.second.jail)) {
            std::string name = std::to_string(i) + ". " + player.second.name;
            if (ImGui::Selectable(name.c_str(), m_votedPlayer == player.second.id)) {
              m_votedPlayer = player.second.id;
            }
            ++i;
          }
        }
        if (ImGui::Selectable("None Of The Above", m_votedPlayer == gf::InvalidId)) {
          m_votedPlayer = gf::InvalidId;
        }

        if (ImGui::Button("Vote", ImVec2(ImGui::GetWindowWidth(), DefaultButtonSize.y))) {
          gf::Log::debug("(GAME) Vote for: %" PRIX64 "\n", m_votedPlayer);

          PemClientChoosePrisoner vote;
          vote.member = m_votedPlayer;
          m_network.send(vote);

          m_alreadyVote = true;
        }
      }
      ImGui::End();
    }

    // Chat window
    m_chat.display(coordinates);

    // Default display
    renderWorldEntities(target, states);
    renderHudEntities(target, states);

    //Start move and play button
    if (m_gamePhase == GamePhase::CapitainElection && m_players[m_scenes.myPlayerId].captain) {
      float characterSize = coordinates.getRelativeCharacterSize(0.05f);
      m_startMoveAndPlayButton.setCharacterSize(characterSize);
      m_startMoveAndPlayButton.setPosition(coordinates.getRelativePoint({0.5f, 0.05f}));
      m_startMoveAndPlayButton.setTextOutlineThickness(characterSize * 0.05f);

      target.draw(m_startMoveAndPlayButton, states);
    }

    ImGui::Render();
    ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
  }

}
