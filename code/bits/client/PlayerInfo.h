#ifndef TMPGAME_PLAYER_INFO_H
#define TMPGAME_PLAYER_INFO_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>

namespace ggj {
  class PlayerInfo: public gf::Entity {
  public:
    PlayerInfo(gf::ResourceManager& resources);

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    void setRole(const std::string &role) {
      m_role = role;
    }

  private:
    gf::Font &m_font;
    std::string m_role;
  };

}


#endif // TMPGAME_PLAYER_INFO_H
