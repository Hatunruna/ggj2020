#include "ServerRoom.h"

#include <cassert>
#include <cinttypes>
#include <algorithm>

#include <gf/Log.h>

#include "Protocol.h"

namespace ggj {

  ServerRoom::ServerRoom(GameFactory& factory)
  : m_factory(factory)
  , m_instance(nullptr)
  {
  }

  void ServerRoom::update(ServerPlayer& player, ProtocolBytes& bytes) {
    assert(player.room == this);

    switch (bytes.getType()) {
      case ClientChangeTeam::type: {
        gf::Log::info("(ROOM) {%" PRIX64 "} Change team.\n", player.id);
        if (isGameStarted()) {
          gf::Log::warning("(ROOM) {%" PRIX64 "} Game already started\n", player.id);
          ServerError error;
          error.reason = ServerError::GameAlreadyStarted;
          player.send(error);
          break;
        }
        if (player.ready) {
          gf::Log::warning("(ROOM) {%" PRIX64 "} Player already ready\n", player.id);
          ServerError error;
          error.reason = ServerError::PlayerAlreadyReady;
          player.send(error);
          break;
        }
        auto data = bytes.as<ClientChangeTeam>();
        if (player.team == data.team) {
          break;
        }
        // Check if team exists
        if (data.team < 0 || data.team >= settings.teams) {
          gf::Log::warning("(ROOM) {%" PRIX64 "} Unknown team\n", player.id);
          ServerError error;
          error.reason = ServerError::UnknownTeam;
          player.send(error);
          break;
        }
        // Check if team is not full
        if (getPlayersCountInTeam(data.team) == settings.playersByTeam) {
          gf::Log::warning("(ROOM) {%" PRIX64 "} Full team\n", player.id);
          ServerError error;
          error.reason = ServerError::FullTeam;
          player.send(error);
          break;
        }
        player.team = data.team;
        // send an acknowledgement to the player
        ServerChangeTeam change;
        change.team = data.team;
        player.send(change);
        // broadcast new state
        broadcastPlayers();
        break;
      }

      case ClientReady::type: {
        gf::Log::info("(ROOM) {%" PRIX64 "} Ready.\n", player.id);
        if (isGameStarted()) {
          gf::Log::warning("(ROOM) {%" PRIX64 "} Game already started\n", player.id);
          ServerError error;
          error.reason = ServerError::GameAlreadyStarted;
          player.send(error);
          break;
        }
        auto data = bytes.as<ClientReady>();
        // Check if the player is in a team
        if (player.team == -1) {
          gf::Log::warning("(ROOM) {%" PRIX64 "} Player not in a team\n", player.id);
          ServerError error;
          error.reason = ServerError::PlayerNotInTeam;
          player.send(error);
          break;
        }
        player.ready = data.ready;
        // send an acknowledgement to the player
        ServerReady ready;
        ready.ready = data.ready;
        player.send(ready);
        // broadcast new state
        broadcastPlayers();
        // Check if everyone is ready
        startGameIfReady();
        break;
      }

      case ClientChatMessage::type: {
        gf::Log::info("(ROOM) {%" PRIX64 "} Chat message.\n", player.id);
        if (isGameStarted()) {
          break;
        }
        // deserialize
        auto in = bytes.as<ClientChatMessage>();
        // broadcast the message
        ServerChatMessage out;
        out.message.origin = player.id;
        out.message.author = player.name;
        out.message.content = std::move(in.content);
        broadcast(out);
        break;
      }
    }

    if (isGameStarted()) {
      if (m_instance->isFinished()) {
        gf::Log::info("(ROOM) {%" PRIX64 "} Stop game in @%" PRIX64 ".\n", player.id, player.room->id);
        ServerStopGame data;
        broadcast(data);
        m_instance = nullptr;
      } else {
        gf::Log::info("(ROOM) {%" PRIX64 "} Forwarding to game in @%" PRIX64 ".\n", player.id, player.room->id);
        m_instance->update(player, bytes);
      }
    }
  }

  void ServerRoom::broadcastPlayers() {
    ServerListRoomPlayers data;
    data.players = getPlayers();
    broadcast(data);
  }

  bool ServerRoom::isGameStarted() {
    return m_instance != nullptr;
  }

  void ServerRoom::startGameIfReady() {
    if (!areAllPlayersReady()) {
      return;
    }

    m_instance = m_factory.get().createInstance(settings);
    cloneTo(*m_instance);
    m_instance->start();

    ServerStartGame data;
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

    if (isGameStarted()) {
      m_instance->removePlayer(player);
    }
  }

}
