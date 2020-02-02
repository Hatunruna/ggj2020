#include <cstdlib>

#include <gf/Log.h>

#include <SFML/Audio.hpp>

#include "bits/client/ClientNetwork.h"
#include "bits/client/Scenes.h"
#include "bits/client/Singletons.h"

#include "config.h"

#include <cassert>
#include <gf/Sleep.h>
#include "common/Protocol.h"

int main(int argc, char *argv[]) {
  ggj::ClientNetwork network;

  // Singletons
  gf::SingletonStorage<ggj::ResourceManager> storageForResourceManager(ggj::gResourceManager);
  ggj::gResourceManager().addSearchDir(TMPGAME_DATA_DIR);

  // Background music
  ggj::gBackgroundMusic.setBuffer(ggj::gResourceManager().getSound("audio/main_theme.ogg"));
  ggj::gBackgroundMusic.setLoop(true);
  ggj::gBackgroundMusic.setVolume(ggj::BackgroundAmbiantVolume);
  ggj::gBackgroundMusic.play();

  ggj::Scenes scenes(network, TMPGAME_DATA_DIR);
  scenes.setClearColor(gf::Color::Black);

  if (argc == 2 && std::string(argv[1]) == "--debug") {
    gf::Log::debug("(CLIENT) run in debug mode\n");

    // Wait connection
    network.connect("localhost");
    gf::sleep(gf::milliseconds(500));
    assert(network.isConnected());

    // Send hello package
    {
      ggj::ClientHello data;
      data.name = "DebugClient";
      network.send(data);
    }

    // Receive ack
    {
      ggj::ProtocolBytes bytes;
      network.queue.wait(bytes);
      assert(bytes.getType() == ggj::ServerHello::type);

      auto data = bytes.as<ggj::ServerHello>();
      scenes.myPlayerId = data.playerId;
    }

    gf::Id roomID = gf::InvalidId;
    {
      ggj::ProtocolBytes bytes;
      network.queue.wait(bytes);
      assert(bytes.getType() == ggj::ServerListRooms::type);

      // Check if room is already created
      auto data = bytes.as<ggj::ServerListRooms>();
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
      ggj::ProtocolBytes bytes;
      network.queue.wait(bytes);
      assert(bytes.getType() == ggj::ServerListPlayers::type);
    }

    // If room was not created
    if (roomID == gf::InvalidId) {
      ggj::ClientCreateRoom data;
      data.name = "DebugRoom";

      ggj::GameInstanceSettings settings;
      settings.playersByTeam = 4;
      settings.teams = 1;

      data.settings = settings;
      network.send(data);
    }
    else {
      ggj::ClientJoinRoom data;
      data.room = roomID;
      network.send(data);
    }

    // Join the room
    {
      gf::Log::debug("Join the room\n");
      ggj::ProtocolBytes bytes;
      network.queue.wait(bytes);
      assert(bytes.getType() == ggj::ServerJoinRoom::type);

      auto data = bytes.as<ggj::ServerJoinRoom>();
      scenes.room.startRoom(data.settings);

      gf::Log::debug("Joined room %lX\n", data.room);
    }

    // Ignore players list room
    {
      ggj::ProtocolBytes bytes;
      network.queue.wait(bytes);
      assert(bytes.getType() == ggj::ServerListRoomPlayers::type);
    }

    // Ignore change team
    {
      ggj::ProtocolBytes bytes;
      network.queue.wait(bytes);
      assert(bytes.getType() == ggj::ServerChangeTeam::type);
    }

    // Ignore players list room
    {
      ggj::ProtocolBytes bytes;
      network.queue.wait(bytes);
      assert(bytes.getType() == ggj::ServerListRoomPlayers::type);
    }

    // Send player ready
    {
      ggj::ClientReady data;
      data.ready = true;
      network.send(data);
    }

    // Ack ready
    {
      ggj::ProtocolBytes bytes;
      network.queue.wait(bytes);
      assert(bytes.getType() == ggj::ServerReady::type);

      auto data = bytes.as<ggj::ServerReady>();
      assert(data.ready);
    }

    std::vector<ggj::PlayerData> players;
    // Update player list
    {
      ggj::ProtocolBytes bytes;
      for(;;) {
        network.queue.wait(bytes);

        if (bytes.getType() == ggj::ServerListRoomPlayers::type) {
          auto data = bytes.as<ggj::ServerListRoomPlayers>();
          players = std::move(data.players);
        }
        else {
          break;
        }
      }

      assert(bytes.getType() == ggj::ServerStartGame::type);
      assert(players.size() == 4u);
      scenes.game.initialize(players);
    }

    scenes.pushScene(scenes.game);
    scenes.run();
  }
  else {
    scenes.pushScene(scenes.intro);
    scenes.run();
  }
  return EXIT_SUCCESS;
}
