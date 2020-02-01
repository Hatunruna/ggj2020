#ifndef TMPGAME_RESOURCE_MANAGER_H
#define TMPGAME_RESOURCE_MANAGER_H

#include <gf/ResourceManager.h>
#include <SFML/Audio.hpp>

namespace ggj {

  class ResourceManager : public gf::ResourceManager {
  public:
    ResourceManager();
    sf::SoundBuffer& getSound(const gf::Path &path);
    sf::Music& getMusic(const gf::Path &path);

  private:
    gf::ResourceCache<sf::SoundBuffer> m_sounds;
    gf::ResourceCache<sf::Music> m_musics;
  };

}

#endif // TMPGAME_RESOURCE_MANAGER_H
