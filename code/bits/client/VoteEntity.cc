#include "VoteEntity.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

namespace pem {

  VoteEntity::VoteEntity(gf::ResourceManager &resources, GameModel &model)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_model(model) {
  }

  void VoteEntity::initialize() {
    m_textWidgets.clear();
    m_container.clear();

    // Add player choice
    for (const auto &entry: m_model.players) {
      gf::TextWidget textWidget(entry.second.name, m_font);
      textWidget.setCallback([this, &entry](){
        // No vote if in jail
        if (m_model.players.at(m_model.myPlayerId).jail) {
          return;
        }

        m_model.hasVoted = true;
        m_model.voteSelection = entry.first;
      });

      m_textWidgets.emplace(entry.first, std::move(textWidget));
      m_container.addWidget(m_textWidgets.at(entry.first));
    }

    // Add null vote
    gf::TextWidget textWidget("None Of The Above", m_font);
    textWidget.setCallback([this](){
      // No vote if in jail
      if (m_model.players.at(m_model.myPlayerId).jail) {
        return;
      }

      m_model.hasVoted = true;
      m_model.voteSelection = gf::InvalidId;
    });

    m_textWidgets.emplace(gf::InvalidId, std::move(textWidget));
    m_container.addWidget(m_textWidgets.at(gf::InvalidId));
  }

  void VoteEntity::updateVoteList() {
    // Update text widgets state
    for (auto &entry: m_model.players) {
      auto &textWidget = m_textWidgets.at(entry.first);
      if (entry.second.jail) {
        // textWidget.setDisabled(); // assert throw if uncommented
      }
      else {
        textWidget.setDefault();
      }
    }
  }

  void VoteEntity::pointTo(gf::Vector2f coords) {
    m_container.pointTo(coords);
  }

  void VoteEntity::triggerAction() {
    m_container.triggerAction();
  }

  void VoteEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    bool inJail = m_model.players.at(m_model.myPlayerId).jail;
    if (m_model.gamePhase != GamePhase::Meeting || m_model.hasVoted || inJail) {
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

    // Display vote for all players
    for (auto &entry: m_model.players) {
      int col = index % 2;
      int row = index / 2;
      ++index;

      auto position = coords.getRelativePoint({ (1.0f / 6.0f) + (col * 1.0f / 3.0f), 0.85f - row * (0.10f) });

      auto &textWidget = m_textWidgets.at(entry.first);
      textWidget.setCharacterSize(characterSize);
      textWidget.setAnchor(gf::Anchor::Center);
      textWidget.setPosition(position);
      textWidget.setDefaultTextColor(gf::Color::White);
      textWidget.setDefaultTextOutlineColor(gf::Color::Black);
      textWidget.setSelectedTextOutlineColor(gf::Color::Black);
      textWidget.setTextOutlineThickness(characterSize * 0.05f);
    }

    position = coords.getRelativePoint({ 1.0f / 3.0f, 0.95f });
    auto &textWidget = m_textWidgets.at(gf::InvalidId);
    textWidget.setCharacterSize(characterSize);
    textWidget.setAnchor(gf::Anchor::Center);
    textWidget.setPosition(position);
    textWidget.setDefaultTextColor(gf::Color::White);
    textWidget.setDefaultTextOutlineColor(gf::Color::Black);
    textWidget.setSelectedTextOutlineColor(gf::Color::Black);
    textWidget.setTextOutlineThickness(characterSize * 0.05f);

    m_container.render(target, states);
  }

}
