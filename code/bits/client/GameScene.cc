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

namespace pem {

  GameScene::GameScene(Scenes& scenes, ClientNetwork& network, gf::ResourceManager& resources)
  : gf::Scene(InitialSize)
  , m_scenes(scenes)
  , m_network(network)
  , m_escapeAction("Escape")
  , m_stars(gf::RectF::fromMinMax({-3000.0f, -1000.0f}, {6000.0f, 3000.0f})) // TODO: set the right box
  , m_ship(resources, m_model)
  , m_info(resources, m_model)
  , m_chat(network, m_model)
  , m_vote(resources, m_model)
  , m_adaptator(m_scenes.getRenderer(), getWorldView())
  {
    // Set default clear color
    setClearColor(gf::Color::Black);

    // Reset game state
    resetGameModel();

    // Set the view size
    setWorldViewSize(WorldSize);
    setWorldViewCenter(WorldSize * 0.5f);
    getWorldView().setViewport(gf::RectF::fromPositionSize({0.0f, 0.0f}, {1.0f, 2.f / 3.f}));

    // Set key action
    m_escapeAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_escapeAction);

    // Add worlds entities
    addWorldEntity(m_ship);
    addWorldEntity(m_stars);

    // Add HUD entities
    addHudEntity(m_info);
    addHudEntity(m_vote);

    // Define song parameters
    m_ambiantBackground.setBuffer(gResourceManager().getSound("audio/ambiant.ogg"));
    m_ambiantBackground.setVolume(BackgroundAmbiantVolume);
    m_ambiantBackground.setLoop(true);
    m_cardShuffle.setBuffer(gResourceManager().getSound("audio/shuffle_card.ogg"));
    m_cardShuffle.setVolume(FxsVolume);
  }

  void GameScene::initialize(const std::vector<PlayerData> &players) {
    // Reset game
    resetGameModel();

    // Initialize players
    for(auto &player: players) {
      ClientPlayerData playerData;
      playerData.id = player.id;
      playerData.name = player.name;
      m_model.players.emplace(player.id, playerData);
    }

    // Set game phase
    m_model.gamePhase = GamePhase::Action;

    // Initialize vote
    m_vote.initialize();

    // Play songs
    gBackgroundMusic.stop();
    m_ambiantBackground.play();
    m_cardShuffle.play();
  }

  void GameScene::doHandleActions(gf::Window& window) {
    gf::unused(window);

    if (m_escapeAction.isActive()) {
      // Stop song
      m_ambiantBackground.stop();
      gBackgroundMusic.play();

      // Go back to main menu
      // TODO: change to go back to lobby
      m_scenes.replaceScene(m_scenes.intro, m_scenes.fadeEffect, gf::seconds(0.4f));
    }
  }

  bool GameScene::doEarlyProcessEvent(gf::Event &event) {
    return ImGui_ImplGF_ProcessEvent(event);
  }

  void GameScene::doProcessEvent(gf::Event &event) {
    m_adaptator.processEvent(event);

    switch (m_model.gamePhase) {
      case GamePhase::Action: {
        // On left click
        if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
          gf::Vector2i mouseCoords = event.mouseButton.coords;
          gf::Vector2f worldCoords = m_scenes.getRenderer().mapPixelToCoords(event.mouseButton.coords, getWorldView());
          auto screenSize = m_scenes.getRenderer().getSize();

          // std::printf("      polygon.addPoint({ %gf, %gf });\n", worldCoords.x, worldCoords.y); // Debug print for hitbox

          // Update player selection
          PlaceType oldPlace = m_model.selectedPlace;
          m_ship.updateMouseCoords(worldCoords);
          m_ship.updateSelectedPlace();

          // If a selected
          if (m_model.selectedPlace != PlaceType::None && m_model.selectedPlace != oldPlace) {
            // Unable to move when we are in prison
            if (m_model.players.at(m_scenes.myPlayerId).jail) {
              m_model.selectedPlace = PlaceType::Prison;

              MessageData message;
              message.origin = gf::InvalidId;
              message.author = "server";
              message.content = "You cannot move around while you are in jail.";

              m_chat.appendMessage(std::move(message));
              break;
            }

            gf::Log::debug("(GAME) Clicked place: %s\n", placeTypeString(m_model.selectedPlace).c_str());
            m_fx.setBuffer(gResourceManager().getSound("audio/foot_steps.ogg"));
            m_fx.setVolume(FxsVolume);
            m_fx.play();
          }

          // Check if a card was selected after select a place
          // TODO: Replace by SpriteWidget
          if (m_model.selectedPlace != PlaceType::None) {
            // Get the clicked card
            m_info.updateSelectedCard(mouseCoords, screenSize);
            if (m_model.selectedCard == -1) {
              break;
            }

            // Can select only release type card when in jail
            if (m_model.players.at(m_scenes.myPlayerId).jail && m_model.cards[m_model.selectedCard] != CardType::Release) {
              m_model.selectedCard = -1;

              MessageData message;
              message.origin = gf::InvalidId;
              message.author = "server";
              message.content = "You can only select \"Release\" card when you are in jail!";

              m_chat.appendMessage(std::move(message));
              break;
            }

            // Send the action to the server
            PemClientMoveAndPlay moveAndPlay;
            moveAndPlay.place = m_model.selectedPlace;
            moveAndPlay.card = m_model.cards[m_model.selectedCard];
            gf::Log::debug("(GAME) Action is: %s -> %s\n", placeTypeString(moveAndPlay.place).c_str(), cardTypeString(moveAndPlay.card).c_str());
            m_network.send(moveAndPlay);

            // Pass to the next phase
            m_model.gamePhase = GamePhase::Resolution;

            // Play fx sound if avaible
            bool playFx = true;
            switch (moveAndPlay.card) {
              case CardType::Block: {
                m_fx.setBuffer(gResourceManager().getSound("audio/block.ogg"));
                break;
              }

              case CardType::Demine: {
                m_fx.setBuffer(gResourceManager().getSound("audio/demine.ogg"));
                break;
              }

              case CardType::FalseAlarm: {
                m_fx.setBuffer(gResourceManager().getSound("audio/alarm.ogg"));
                break;
              }

              case CardType::PlaceBomb0:
              case CardType::PlaceBomb1:
              case CardType::PlaceBomb2: {
                m_fx.setBuffer(gResourceManager().getSound("audio/bomb_armed.ogg"));
                break;
              }

              case CardType::Reinforce1:
              case CardType::Reinforce2: {
                m_fx.setBuffer(gResourceManager().getSound("audio/reinforce.ogg"));
                break;
              }

              case CardType::Release: {
                m_fx.setBuffer(gResourceManager().getSound("audio/gate.ogg"));
                break;
              }

              case CardType::Repair:
              case CardType::FalseRepair1:
              case CardType::FalseRepair2: {
                m_fx.setBuffer(gResourceManager().getSound("audio/repair.ogg"));
                break;
              }

              case CardType::SetupJammer: {
                m_fx.setBuffer(gResourceManager().getSound("audio/jammer.ogg"));
                break;
              }

              default:
                playFx = false;
                break;
            }

            // If a song is avaible
            if (playFx) {
              m_fx.play();
            }
          }
        }
        else if (event.type == gf::EventType::MouseMoved) {
          // Update mouse location for ship entity if not in jail
          if (!(m_model.players.at(m_scenes.myPlayerId).jail)) {
            gf::Vector2f worldCoords = m_scenes.getRenderer().mapPixelToCoords(event.mouseCursor.coords, getWorldView());
            m_ship.updateMouseCoords(worldCoords);
          }
        }
        break;
      }

      case GamePhase::Meeting:
        switch (event.type) {
        case gf::EventType::MouseMoved:
          m_vote.pointTo(m_scenes.getRenderer().mapPixelToCoords(event.mouseCursor.coords));
          break;

        case gf::EventType::MouseButtonPressed:
          // Update vote entity
          m_vote.pointTo(m_scenes.getRenderer().mapPixelToCoords(event.mouseButton.coords));
          m_vote.triggerAction();

          if (m_model.hasVoted) {
            auto id = m_model.voteSelection;
            gf::Log::debug("(GAME) voted for %lX\n", id);
            PemClientChoosePrisoner vote;
            vote.member = id;
            m_network.send(vote);
          }
          break;

        default:
          break;
        }
        break;

      default:
        break;
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
        case ServerChatMessage::type: {
          gf::Log::debug("(GAME) receive ServerChatMessage\n");
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

        case PemServerInitRole::type: {
          gf::Log::debug("(GAME) receive PemServerInitRole\n");

          auto data = bytes.as<PemServerInitRole>();
          if(data.role == CrewType::Rebel){
            gf::Log::debug("(GAME) Role is Rebel\n");
          }
          else{
            gf::Log::debug("(GAME) Role is Protector\n");
          }

          m_model.role = data.role;
          m_model.cards = data.cards;
          break;
        }

        case PemServerStartMoveAndPlay::type: {
          gf::Log::debug("(GAME) receive PemServerStartMoveAndPlay\n");

          resetTurn();
          break;
        }

        case PemServerUpdateShip::type: {
          gf::Log::debug("(GAME) receive PemServerUpdateShip\n");

          auto data = bytes.as<PemServerUpdateShip>();
          for (auto &entry: data.states) {
            m_model.placeLocations.at(entry.first).working = entry.second;
          }

          break;
        }

        case PemServerStartVoteForPrisoner::type: {
          gf::Log::debug("(GAME) receive PemServerStartVoteForPrisoner\n");

          m_model.gamePhase = GamePhase::Meeting;

          m_vote.updateVoteList();
          break;
        }

        case PemServerChoosePrisoner::type: {
          gf::Log::debug("(GAME) receive PemServerChoosePrisoner\n");
          auto data = bytes.as<PemServerChoosePrisoner>();

          MessageData message;
          message.origin = gf::InvalidId;
          message.author = "server";

          auto it = m_model.players.find(data.member);
          if (it != m_model.players.end()) {
            it->second.jail = true;
            message.content = it->second.name + " is now jailed";
          } else {
            message.content = "No one has been in prison";
          }
          m_chat.appendMessage(std::move(message));

          m_vote.updateVoteList();
          break;
        }

        case PemServerReleasePrisoner::type: {
          gf::Log::debug("(GAME) receive PemServerReleasePrisoner\n");
          auto data = bytes.as<PemServerReleasePrisoner>();

          // NOTE: leak of information
          // When a player relased another player, the server send a packet
          // to tell that this player is free. But this message was broacasted
          // so all players know this information even if it isn't displayed.
          // But we are obliged to broacast this information to update the jail
          // status of all player.
          // So the question is: Do we need to know the status of all players or just our own?

          m_model.players.at(data.prisoner).jail = false;

          MessageData message;
          message.origin = gf::InvalidId;
          message.author = "server";
          if (data.prisoner == m_scenes.myPlayerId) {
            if (data.deliverer == gf::InvalidId) {
              message.content = "You were released from prison";
            }
            else {
              auto member = m_model.players.at(data.deliverer);
              message.content = member.name + " released you from prison";
            }
          }
          else {
            auto member = m_model.players.at(data.prisoner);
            message.content = member.name + " was released from prison";
          }

          m_chat.appendMessage(std::move(message));

          m_vote.updateVoteList();
          break;
        }


        // case PemServerResolution::type: {
        //   gf::Log::debug("(GAME) receive PemServerResolution\n");
        //   auto data = bytes.as<PemServerResolution>();

        //   for(auto &resolution: data.conclusion) {
        //     MessageData message;
        //     message.origin = gf::InvalidId;
        //     message.author = "server";

        //     switch (resolution.type) {
        //       case ResolutionType::Examine: {
        //         if (resolution.bomb) {
        //           message.content = "There is a bomb.";
        //         } else {
        //           message.content = "There is no bomb.";
        //         }
        //         break;
        //       }
        //       case ResolutionType::Hide: {
        //         if (resolution.members.size() > 0) {
        //           message.content = "I saw ";
        //           for (auto &playerID: resolution.members) {
        //             message.content += m_players.at(playerID).name + " ";
        //           }
        //           message.content += " hiding.";
        //         } else {
        //           message.content = "I saw nobody.";
        //         }
        //         break;
        //       }
        //       case ResolutionType::Track: {
        //         if (resolution.members.size() > 0) {
        //           message.content = "I detected ";
        //           for (auto &playerID: resolution.members) {
        //             message.content += m_players.at(playerID).name + " ";
        //           }
        //         } else {
        //           message.content = "I detected nobody.";
        //         }
        //         break;
        //       }
        //       case ResolutionType::Block: {
        //         message.content = "The room was blocked.";
        //         break;
        //       }
        //       case ResolutionType::Release: {
        //         for (auto &playerID: resolution.members) {
        //           m_players[playerID].jail = false;
        //           message.content += m_players[playerID].name + " ";
        //         }
        //         message.content += "are released from prison.";
        //         break;
        //       }
        //     }

        //     m_chat.appendMessage(std::move(message));
        //   }

        //   break;
        // }

        // case PemServerUpdateHand::type: {
        //   gf::Log::debug("(GAME) receive PemServerUpdateHand\n");

        //   auto data = bytes.as<PemServerUpdateHand>();
        //   m_info.replaceCard(data.card);

        //   break;
        // }

        case PemServerMissionStatus::type: {
          gf::Log::debug("(GAME) receive PemServerMissionStatus\n");
          auto data = bytes.as<PemServerMissionStatus>();

          m_model.turn = data.turn;
          m_model.distance = data.distance;

          if (data.distance <= 0.0f || data.turn <= 0) {
            m_model.gamePhase = GamePhase::CapitainElection;
          }
          break;
        }
      }
    }
  }

  void GameScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coordinates(target);

    // Chat window
    m_chat.display(coordinates);

    // Default display
    renderWorldEntities(target, states);
    renderHudEntities(target, states);

    ImGui::Render();
    ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
  }

  void GameScene::resetGameModel() {
    m_model.myPlayerId = m_scenes.myPlayerId;
    m_model.players.clear();
    m_model.gamePhase = GamePhase::Action;
    m_model.initializePlaceLocations();
    m_model.selectedPlace = PlaceType::None;
    m_model.selectedCard = -1;

    // Must be initialize by the server
    m_model.distance = -1.0f;
    m_model.turn = -1;

    m_model.hasVoted = false;
    m_model.voteSelection = gf::InvalidId;
  }

  void GameScene::resetTurn() {
    // Change the game phase
    m_model.gamePhase = GamePhase::Action;

    // Unselect cards
    m_model.selectedCard = -1;

    // A player in jail stay in jail
    if (m_model.players.at(m_scenes.myPlayerId).jail) {
      m_model.selectedPlace = PlaceType::Prison;
    }
    else {
      m_model.selectedPlace = PlaceType::None;
    }

    // Reset vote
    m_model.hasVoted = false;
    m_model.voteSelection = gf::InvalidId;

    // Tell to the player
    MessageData message;
    message.origin = gf::InvalidId;
    message.author = "server";
    message.content = "A new day of duty has begun!";
    m_chat.appendMessage(std::move(message));
  }

}
