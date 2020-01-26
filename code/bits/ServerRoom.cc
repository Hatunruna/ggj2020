#include "ServerRoom.h"

#include <cinttypes>

#include <gf/Log.h>

#include "Protocol.h"

namespace ggj {

  void ServerRoom::update(ServerPlayer& player, ProtocolBytes& bytes) {
    switch (bytes.getType()) {
      case ClientChangeTeam::type: {
        gf::Log::info("(ROOM) {%" PRIX64 "} Change team.\n", player.id);
        auto data = bytes.as<ClientChangeTeam>();
        // TODO: check if team exists
        // TODO: check if team is not full
        player.team = data.team;
        // send an acknowledgement to the player
        ServerChangeTeam change;
        change.team = data.team;
        player.send(change);
        // broadcast new state
        broadcastPlayers();
        break;
      }
    }
  }

  void ServerRoom::broadcastPlayers() {
    ServerListRoomPlayers data;
    data.players = getPlayers();
    broadcast(data);
  }

  void ServerRoom::doAddPlayer(ServerPlayer& player) {
    gf::Log::info("(ROOM) {%" PRIX64 "} Adding player.\n", player.id);
    player.room = this;
    player.team = -1;
    player.ready = false;
    broadcastPlayers();
  }

  void ServerRoom::doRemovePlayer(ServerPlayer& player) {
    gf::Log::info("(ROOM) {%" PRIX64 "} Removing player.\n", player.id);
    player.room = nullptr;
    player.team = -1;
    player.ready = false;
    broadcastPlayers();
  }

}
