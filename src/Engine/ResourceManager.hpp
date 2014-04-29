#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <unordered_map>

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"

class ResourceManager {
 public:
    ResourceManager();
    ~ResourceManager();

    // Textures
    sf::Texture* loadTexture(const std::string& file);
    sf::Texture* getTexture(const std::string& file);
    void freeTexture(const std::string& file);

    // Fonts
    sf::SoundBuffer* loadSoundBuffer(const std::string& file);
    sf::SoundBuffer* getSoundBuffer(const std::string& file);
    void freeSoundBuffer(const std::string& file);

    // Fonts
    sf::Font* loadFont(const std::string& file);
    sf::Font* getFont(const std::string& file);
    void freeFont(const std::string& file);

 private:
    std::unordered_map<std::string, sf::Texture*> textures_;
    std::unordered_map<std::string, sf::SoundBuffer*> sound_buffers_;
    std::unordered_map<std::string, sf::Font*> fonts_;
};

#endif  // RESOURCE_MANAGER_HPP
