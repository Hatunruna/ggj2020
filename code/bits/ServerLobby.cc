#include "ServerLobby.h"

#include <cassert>
#include <cinttypes>

#include <gf/Log.h>

#include "Protocol.h"

namespace ggj {

  ServerLobby::ServerLobby(gf::Random& random, GameFactory& factory, const GameSettings& settings)
  : m_random(random)
  , m_factory(factory)
  , m_settings(settings)
  {
  }

  void ServerLobby::update(ServerPlayer& player, ProtocolBytes& bytes) {
    switch (bytes.getType()) {
      case ClientHello::type: {
        gf::Log::info("(LOBBY) {%" PRIX64 "} Hello.\n", player.id);
        auto data = bytes.as<ClientHello>();
        player.name = data.name;
        // send an acknowledgement to the player
        ServerHello hello;
        hello.settings = m_settings;
        player.send(hello);
        // send list of rooms
        ServerListRooms list;
        list.rooms = getRooms();
        player.send(list);
        // broadcast new information
        broadcastPlayers();
        break;
      }

      case ClientChangeName::type: {
        gf::Log::info("(LOBBY) {%" PRIX64 "} Change name.\n", player.id);
        auto data = bytes.as<ClientChangeName>();
        player.name = data.name;
        // send an acknowledgement to the player
        ServerChangeName change;
        change.name = data.name;
        player.send(change);
        // broadcast new information
        broadcastPlayers();
        break;
      }

      case ClientCreateRoom::type: {
        auto id = m_random.get().computeId();
        gf::Log::info("(LOBBY) {%" PRIX64 "} Create room @%" PRIX64 "\n", player.id, id);
        // deserialize
        auto data = bytes.as<ClientCreateRoom>();
        // create new room
        ServerRoom roomInstance;
        roomInstance.id = id;
        roomInstance.name = std::move(data.name);
        roomInstance.settings = data.settings; // TODO: clamp
        // add it to the lobby
        auto res = m_rooms.emplace(id, std::move(roomInstance));
        assert(res.second);
        auto& room = res.first->second;
        // send an acknowledgement to the player
        ServerJoinRoom join;
        join.room = id;
        join.settings = data.settings;
        player.send(join);
        // move player to the room
        removePlayer(player);
        room.addPlayer(player);
        // broadcast new information
        broadcastRooms();
        break;
      }

      case ClientQueryRoom::type: {
        gf::Log::info("(LOBBY) {%" PRIX64 "} Query room.\n", player.id);
        // deserialize
        auto data = bytes.as<ClientQueryRoom>();
        // find the room
        auto it = m_rooms.find(data.room);
        if (it == m_rooms.end()) {
          gf::Log::warning("(LOBBY) {%" PRIX64 "} Querying an unknown room @%" PRIX64 "\n", player.id, data.room);
          break;
        }
        auto& room = it->second;
        // send the answer
        ServerAnswerRoom answer;
        answer.room = room.id;
        answer.name = room.name;
        answer.settings = room.settings;
        player.send(answer);
        break;
      }

      case ClientJoinRoom::type: {
        gf::Log::info("(LOBBY) {%" PRIX64 "} Join room.\n", player.id);
        // check if player is not in a room
        if (player.room != nullptr) {
          gf::Log::warning("(LOBBY) {%" PRIX64 "} Already in room @%" PRIX64 "\n", player.id, player.room->id);
          break;
        }
        // deserialize
        auto data = bytes.as<ClientJoinRoom>();
        // find the room
        auto it = m_rooms.find(data.room);
        if (it == m_rooms.end()) {
          gf::Log::warning("(LOBBY) {%" PRIX64 "} Unknown room @%" PRIu64 "\n", player.id, data.room);
          break;
        }
        auto& room = it->second;
        // check if the room can accept more players
        // TODO
        // send the answer
        ServerJoinRoom join;
        join.room = room.id;
        join.settings = room.settings;
        player.send(join);
        // move player to the room
        removePlayer(player);
        room.addPlayer(player);
        break;
      }

      case ClientLeaveRoom::type: {
        gf::Log::info("(LOBBY) {%" PRIX64 "} Leave room.\n", player.id);
        // TODO: check if the player is in the room
        auto& room = *player.room;
        room.removePlayer(player);
        checkEmptyRoom(room);
        addPlayer(player);
        // send an acknowledgement to the player
        ServerLeaveRoom leave;
        player.send(leave);
        // send player the list of rooms
        ServerListRooms list;
        list.rooms = getRooms();
        player.send(list);
        // broadcast new information
        broadcastPlayers();
        break;
      }

      case ClientChatMessage::type: {
        gf::Log::info("(LOBBY) {%" PRIX64 "} Chat message.\n", player.id);
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

    if (player.room != nullptr) {
      gf::Log::info("(LOBBY) {%" PRIX64 "} Forwarding to room.\n", player.id);
      player.room->update(player, bytes);
    }

  }

  std::vector<RoomData> ServerLobby::getRooms() {
    std::vector<RoomData> list;

    for (auto& kv : m_rooms) {
      auto& room  = kv.second;
      RoomData data;
      data.id = room.id;
      data.name = room.name;
      list.push_back(std::move(data));
    }

    return list;
  }

  void ServerLobby::broadcastRooms() {
    ServerListRooms data;
    data.rooms = getRooms();
    broadcast(data);
  }

  void ServerLobby::broadcastPlayers() {
    ServerListPlayers data;
    data.players = getPlayers();
    broadcast(data);
  }

  void ServerLobby::doAddPlayer(ServerPlayer& player) {
    gf::Log::info("(LOBBY) {%" PRIX64 "} Adding player.\n", player.id);
  }

  void ServerLobby::doRemovePlayer(ServerPlayer& player) {
    gf::Log::info("(LOBBY) {%" PRIX64 "} Removing player.\n", player.id);

    if (player.room != nullptr) {
      auto& room = *player.room;
      room.removePlayer(player);
      checkEmptyRoom(room);
    }

    broadcastPlayers();
  }

  void ServerLobby::checkEmptyRoom(ServerRoom& room) {
    if (!room.isEmpty()) {
      return;
    }

    auto count = m_rooms.erase(room.id);
    assert(count == 1);

    broadcastRooms();
  }

}
