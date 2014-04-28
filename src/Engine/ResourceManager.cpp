#include "Engine/ResourceManager.hpp"

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
    //  Delete the textures
    for (auto iter = textures_.begin(); iter != textures_.end(); iter++) {
        delete iter->second;
    }

    //  Delete the fonts
    for (auto iter = fonts_.begin(); iter != fonts_.end(); iter++) {
        delete iter->second;
    }
}

//////////////////////////////////////
//  Textures
//////////////////////////////////////

sf::Texture* ResourceManager::loadTexture(const std::string& file) {
    if (textures_.find(file) != textures_.end())
        return textures_[file];

    sf::Texture *texture = new sf::Texture();
    if (texture->loadFromFile(file)) {
        textures_[file] = texture;
    } else {
        fprintf(stderr, "Cant load the texture %s.\n", file.c_str());
    }

    return texture;   
}

sf::Texture* ResourceManager::getTexture(const std::string& file) {
    if (textures_.find(file) != textures_.end())
        return textures_[file];
    else {
        fprintf(stderr, "Can't find the texture %s.\n", file.c_str());
        return nullptr;
    }
}

void ResourceManager::freeTexture(const std::string& file) {
    auto iter = textures_.find(file);
    if (iter != textures_.end()) {
        delete iter->second;
        textures_.erase(iter);
    } else {
        fprintf(stderr, "Can't find the texture %s.\n", file.c_str());
    }
}

//////////////////////////////////////
//  Fonts
//////////////////////////////////////

sf::Font* ResourceManager::loadFont(const std::string& file) {
    if (fonts_.find(file) != fonts_.end())
        return fonts_[file];

    sf::Font *font = new sf::Font();
    if (font->loadFromFile(file)) {
        fonts_[file] = font;
    } else {
        fprintf(stderr, "Cant load the font %s.\n", file.c_str());
    }

    return font;   
}

sf::Font* ResourceManager::getFont(const std::string& file) {
    if (fonts_.find(file) != fonts_.end())
        return fonts_[file];
    else {
        fprintf(stderr, "Can't find the font %s.\n", file.c_str());
        return nullptr;
    }
}

void ResourceManager::freeFont(const std::string& file) {
    auto iter = fonts_.find(file);
    if (iter != fonts_.end()) {
        delete iter->second;
        fonts_.erase(iter);
    } else {
        fprintf(stderr, "Can't find the font %s.\n", file.c_str());
    }
}
