#include "ServerLobby.h"

#include "Protocol.h"

namespace ggj {

  void ServerLobby::update(ServerPlayer& player, ProtocolBytes& bytes) {
    switch (bytes.getType()) {
      case ClientCreateRoom::type: {
        auto data = bytes.as<ClientCreateRoom>();
        // TODO
      }

    }
  }

}
