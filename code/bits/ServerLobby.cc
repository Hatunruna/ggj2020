#include "ServerLobby.h"

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
        // deserialize
        auto data = bytes.as<ClientCreateRoom>();
        // create new room
        ServerRoom room;
        auto id = m_random.get().computeId();
        room.id = id;
        room.name = std::move(data.name);
        room.settings = data.settings;
        // add it to the lobby
        auto res = m_rooms.emplace(id, std::move(room));
        assert(res.second);
        // move player to the room
        removePlayer(player);
        auto& actualRoom = res.first->second;
        actualRoom.addPlayer(player);
        // send an acknowledgement to the player
        ServerJoinRoom join;
        join.room = id;
        player.send(join);
        // broadcast new information
        broadcastRooms();
        broadcastPlayers();
        break;
      }

      case ClientQueryRoom::type: {
        // deserialize
        auto data = bytes.as<ClientQueryRoom>();
        // find the room
        auto it = m_rooms.find(data.room);
        if (it == m_rooms.end()) {
          gf::Log::warning("Querying an unknown room: %" PRIu64, data.room);
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
        // TODO
        break;
      }

      case ClientChatMessage::type: {
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
  }

  void ServerLobby::doRemovePlayer(ServerPlayer& player) {
    if (player.room != nullptr) {
      player.room->removePlayer(player);
    }

    broadcastPlayers();
  }

}
