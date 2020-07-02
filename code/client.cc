#include <cstdlib>

#include <gf/Log.h>
#include <gf/SharedGLContext.h>

#include <SFML/Audio.hpp>

#include "bits/client/ClientNetwork.h"
#include "bits/client/Scenes.h"
#include "bits/client/Singletons.h"

#include "config.h"

#include <cassert>
#include <gf/Sleep.h>
#include "common/Protocol.h"

int main(int argc, char *argv[]) {
  pem::ClientNetwork network;

  // Singletons
  // Ressource Manager only for audio...
  gf::SingletonStorage<pem::ResourceManager> storageForResourceManager(pem::gResourceManager);
  pem::gResourceManager().addSearchDir(PEM_DATA_DIR);

  // Background music
  pem::gBackgroundMusic.setBuffer(pem::gResourceManager().getSound("audio/main_theme.ogg"));
  pem::gBackgroundMusic.setLoop(true);
  pem::gBackgroundMusic.setVolume(pem::BackgroundAmbiantVolume);
  pem::gBackgroundMusic.play();

  // Start graphics
  pem::Scenes scenes(PEM_DATA_DIR);

  if (argc == 3 && std::string(argv[1]) == "--debug") {
    scenes.loadingAllAssets(network);
    gf::Log::debug("(CLIENT) run in debug mode\n");

    // Wait connection
    network.connect("localhost");
    gf::sleep(gf::milliseconds(500));
    assert(network.isConnected());

    // Send hello package
    {
      pem::ClientHello data;
      std::string name(argv[2]);
      assert(!name.empty());
      data.name = name;
      network.send(data);
    }

    // Receive ack
    {
      gf::Packet packet;
      network.queue.wait(packet);
      assert(packet.getType() == pem::ServerHello::type);

      auto data = packet.as<pem::ServerHello>();
      scenes.myPlayerId = data.playerId;
    }

    gf::Id roomID = gf::InvalidId;
    {
      gf::Packet packet;
      network.queue.wait(packet);
      assert(packet.getType() == pem::ServerListRooms::type);

      // Check if room is already created
      auto data = packet.as<pem::ServerListRooms>();
      for (auto& room : data.rooms) {
        if (room.name == "DebugRoom") {
          roomID = room.id;
          gf::Log::debug("Found room %lX\n", roomID);
          break;
        }
      }
    }

    // Ignore players list
    {
      gf::Packet packet;
      network.queue.wait(packet);
      assert(packet.getType() == pem::ServerListPlayers::type);
    }

    // If room was not created
    if (roomID == gf::InvalidId) {
      pem::ClientCreateRoom data;
      data.name = "DebugRoom";

      pem::GameInstanceSettings settings;
      settings.playersByTeam = 4;
      settings.teams = 1;

      data.settings = settings;
      network.send(data);
    }
    else {
      pem::ClientJoinRoom data;
      data.room = roomID;
      network.send(data);
    }

    // Join the room
    {
      gf::Log::debug("Join the room\n");
      gf::Packet packet;
      network.queue.wait(packet);
      assert(packet.getType() == pem::ServerJoinRoom::type);

      auto data = packet.as<pem::ServerJoinRoom>();
      scenes.room->startRoom(data.settings);

      gf::Log::debug("Joined room %lX\n", data.room);
    }

    // Ignore players list room
    {
      gf::Packet packet;
      network.queue.wait(packet);
      assert(packet.getType() == pem::ServerListRoomPlayers::type);
    }

    // Ignore change team
    {
      gf::Packet packet;
      network.queue.wait(packet);
      assert(packet.getType() == pem::ServerChangeTeam::type);
    }

    // Ignore players list room
    {
      gf::Packet packet;
      network.queue.wait(packet);
      assert(packet.getType() == pem::ServerListRoomPlayers::type);
    }

    // Send player ready
    {
      pem::ClientReady data;
      data.ready = true;
      network.send(data);
    }

    // Ack ready
    {
      gf::Packet packet;
      network.queue.wait(packet);
      assert(packet.getType() == pem::ServerReady::type);

      auto data = packet.as<pem::ServerReady>();
      assert(data.ready);
    }

    std::vector<pem::PlayerData> players;
    // Update player list
    {
      gf::Packet packet;
      for(;;) {
        network.queue.wait(packet);

        if (packet.getType() == pem::ServerListRoomPlayers::type) {
          auto data = packet.as<pem::ServerListRoomPlayers>();
          players = std::move(data.players);
        }
        else {
          break;
        }
      }

      assert(packet.getType() == pem::ServerStartGame::type);
      assert(players.size() == 4u);
      scenes.game->initialize(players);
    }

    scenes.pushScene(*scenes.game);
    scenes.run();
  }
  else {
    auto loadingSplashScreen = std::async(std::launch::async, [&scenes, &network]() {
      gf::SharedGLContext glContext(scenes.getWindow());
      scenes.loadingSplashScreen();
      scenes.loadingMainAssets(network);
    });
    scenes.pushScene(*scenes.splashScreen);
    scenes.run();
  }
  return EXIT_SUCCESS;
}
