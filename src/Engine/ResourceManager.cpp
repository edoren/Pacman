#include "Engine/ResourceManager.hpp"

ResourceManager::ResourceManager(const std::string& working_dir)
       : working_dir_(working_dir) {
}

ResourceManager::~ResourceManager() {
    //  Delete the textures
    for (auto iter = textures_.begin(); iter != textures_.end(); iter++) {
        delete iter->second;
    }

    //  Delete the sound buffers
    for (auto iter = sound_buffers_.begin(); iter != sound_buffers_.end(); iter++) {
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
    if (texture->loadFromFile(working_dir_ + file)) {
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
//  Sound Buffers
//////////////////////////////////////

sf::SoundBuffer* ResourceManager::loadSoundBuffer(const std::string& file) {
    if (sound_buffers_.find(file) != sound_buffers_.end())
        return sound_buffers_[file];

    sf::SoundBuffer *sound_buffer = new sf::SoundBuffer();
    if (sound_buffer->loadFromFile(working_dir_ + file)) {
        sound_buffers_[file] = sound_buffer;
    } else {
        fprintf(stderr, "Cant load the sound_buffer %s.\n", file.c_str());
    }

    return sound_buffer;   
}

sf::SoundBuffer* ResourceManager::getSoundBuffer(const std::string& file) {
    if (sound_buffers_.find(file) != sound_buffers_.end())
        return sound_buffers_[file];
    else {
        fprintf(stderr, "Can't find the sound buffer %s.\n", file.c_str());
        return nullptr;
    }
}

void ResourceManager::freeSoundBuffer(const std::string& file) {
    auto iter = sound_buffers_.find(file);
    if (iter != sound_buffers_.end()) {
        delete iter->second;
        sound_buffers_.erase(iter);
    } else {
        fprintf(stderr, "Can't find the sound buffer %s.\n", file.c_str());
    }
}


//////////////////////////////////////
//  Fonts
//////////////////////////////////////

sf::Font* ResourceManager::loadFont(const std::string& file) {
    if (fonts_.find(file) != fonts_.end())
        return fonts_[file];

    sf::Font *font = new sf::Font();
    if (font->loadFromFile(working_dir_ + file)) {
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


//////////////////////////////////////
//  Directory Tools
//////////////////////////////////////

const std::string& ResourceManager::getWorkingDirectory() {
    return working_dir_;
}
