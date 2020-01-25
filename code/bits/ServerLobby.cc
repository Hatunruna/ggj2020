#include "ServerLobby.h"

#include "Protocol.h"

namespace ggj {

  ServerLobby::ServerLobby(gf::Random& random)
  : m_random(random)
  {
  }

  void ServerLobby::update(ServerPlayer& player, ProtocolBytes& bytes) {
    switch (bytes.getType()) {
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
    ServerListRooms data;
    data.rooms = getRooms();
    player.send(data);

    broadcastPlayers();
  }

  void ServerLobby::doRemovePlayer(ServerPlayer& player) {
    if (player.room != nullptr) {
      player.room->removePlayer(player);
    }

    broadcastPlayers();
  }

}
