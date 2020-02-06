#ifndef PEM_VOTE_ENTITY_H
#define PEM_VOTE_ENTITY_H

#include <map>
#include <string>

#include <gf/Entity.h>
#include <gf/Event.h>
#include <gf/Id.h>
#include <gf/ResourceManager.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

#include "GameModel.h"

namespace pem {
  class VoteEntity: public gf::Entity {
  public:
    VoteEntity(gf::ResourceManager &resources, GameModel &model);

    void initialize();
    void updateVoteList();

    void pointTo(gf::Vector2f coords);
    void triggerAction();

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font &m_font;
    std::map<gf::Id, gf::TextWidget> m_textWidgets;
    gf::WidgetContainer m_container;

    GameModel &m_model;

    // std::map<gf::Id, gf::TextWidget> m_voters;
    // gf::Id m_selected;
    // bool m_hasVoted;
  };
}

#endif // PEM_VOTE_ENTITY_H
