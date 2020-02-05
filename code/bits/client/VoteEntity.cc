#include "VoteEntity.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

namespace pem {

  VoteEntity::VoteEntity(gf::ResourceManager &resources)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_selected(gf::InvalidId)
  , m_hasVoted(true) {
  }

  void VoteEntity::changeVoterList(const std::map<gf::Id, ClientPlayerData> &voterList) {
    m_hasVoted = false;
    m_selected = gf::InvalidId;

    m_container.clear();
    m_voters.clear();

    for (auto &voter: voterList) {
      auto entry = m_voters.emplace(voter.first, gf::TextWidget(voter.second.name, m_font));
      assert(entry.second);

      gf::Log::debug("Voter VoteEntity %lX\n", voter.first);

      auto voterId = voter.first;
      entry.first->second.setCallback([this, voterId](){
        m_hasVoted = true;
        gf::Log::debug("Voter callback %lX\n", voterId);
        m_selected = voterId;
      });
      m_container.addWidget(entry.first->second);
    }

    auto entry = m_voters.emplace(gf::InvalidId, gf::TextWidget("None Of The Above", m_font));
    assert(entry.second);
    entry.first->second.setCallback([this, &entry](){
      m_hasVoted = true;
      m_selected = gf::InvalidId;
    });
    m_container.addWidget(entry.first->second);
  }

  void VoteEntity::pointTo(gf::Vector2f coords) {
    m_container.pointTo(coords);
  }

  bool VoteEntity::triggerAction() {
    m_container.triggerAction();

    return m_hasVoted;
  }

  void VoteEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_hasVoted == true) {
      return;
    }

    gf::Coordinates coords(target);
    unsigned characterSize = coords.getRelativeCharacterSize(0.075f);
    int index = 0;

    auto position = coords.getRelativePoint({ 0.025f, 0.65f });
    gf::Text text("Choose a prisoner:", m_font);
    text.setCharacterSize(characterSize);
    text.setAnchor(gf::Anchor::CenterLeft);
    text.setPosition(position);
    text.setColor(gf::Color::White);
    text.setOutlineColor(gf::Color::Black);
    text.setOutlineThickness(characterSize * 0.05f);
    target.draw(text, states);

    for (auto &entry: m_voters) {
      if (entry.first == gf::InvalidId) {
        continue;
      }

      int col = index % 2;
      int row = index / 2;
      ++index;

      auto position = coords.getRelativePoint({ (1.0f / 6.0f) + (col * 1.0f / 3.0f), 0.85f - row * (0.10f) });

      auto &textWidget = entry.second;
      textWidget.setCharacterSize(characterSize);
      textWidget.setAnchor(gf::Anchor::Center);
      textWidget.setPosition(position);
      textWidget.setDefaultTextColor(gf::Color::White);
      textWidget.setDefaultTextOutlineColor(gf::Color::Black);
      textWidget.setSelectedTextOutlineColor(gf::Color::Black);
      textWidget.setTextOutlineThickness(characterSize * 0.05f);
      target.draw(textWidget, states);
    }

    position = coords.getRelativePoint({ 1.0f / 3.0f, 0.95f });
    auto &textWidget = m_voters.at(gf::InvalidId);
    textWidget.setCharacterSize(characterSize);
    textWidget.setAnchor(gf::Anchor::Center);
    textWidget.setPosition(position);
    textWidget.setDefaultTextColor(gf::Color::White);
    textWidget.setDefaultTextOutlineColor(gf::Color::Black);
    textWidget.setSelectedTextOutlineColor(gf::Color::Black);
    textWidget.setTextOutlineThickness(characterSize * 0.05f);
    target.draw(textWidget, states);
  }

}
