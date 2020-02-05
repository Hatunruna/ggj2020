#ifndef PEM_RESOURCE_MANAGER_H
#define PEM_RESOURCE_MANAGER_H

#include <gf/ResourceManager.h>
#include <SFML/Audio.hpp>

namespace pem {

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

#endif // PEM_RESOURCE_MANAGER_H
