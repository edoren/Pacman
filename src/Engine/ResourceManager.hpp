#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <unordered_map>

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Font.hpp"

class ResourceManager {
 public:
    ResourceManager();
    ~ResourceManager();

    // Textures
    sf::Texture* loadTexture(const std::string& texture);
    sf::Texture* getTexture(const std::string& texture);
    void freeTexture(const std::string& texture);

    // Fonts
    sf::Font* loadFont(const std::string& font);
    sf::Font* getFont(const std::string& font);
    void freeFont(const std::string& texture);

 private:
    std::unordered_map<std::string, sf::Texture*> textures_;
    std::unordered_map<std::string, sf::Font*> fonts_;
};

#endif  // RESOURCE_MANAGER_HPP
