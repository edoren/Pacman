#ifndef ANIMATOR_JSON_HPP
#define ANIMATOR_JSON_HPP

////////////////////////////////
// Headers                    //
////////////////////////////////

#include <string>

#include "jansson.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include "Engine/Utils/Clock.hpp"

class AnimatorJson : public sf::Sprite {
 public:
    AnimatorJson();
    virtual ~AnimatorJson();
    
    int loadJsonString(const char* json_string, sf::Texture* texture = nullptr);
    int loadJsonString(const std::string& json_string, sf::Texture* texture = nullptr);

    int loadJsonFile(const char* json_file, sf::Texture* texture = nullptr);
    int loadJsonFile(const std::string& json_file, sf::Texture* texture = nullptr);

    int setAnimation(const char* animation_name);
    int setAnimation(const std::string& animation_name);

    void setFrame(int index);

    void updateAnimation();

    void pauseAnimation();
    void resumeAnimation();
    void restartAnimation();

 private:
    ////////////////////////////////
    // Atributes                  //
    ////////////////////////////////

    std::string rel_path_;

    bool external_texture_;
    sf::Texture* sprite_texture_;

    int frame_index_, frame_duration_;
    Clock clock_;

    json_t *root_, *animation_;

    ////////////////////////////////
    // Private members            //
    ////////////////////////////////

    int loadJsonType(json_t *data, sf::Texture* texture = nullptr);
};

#endif  // ANIMATOR_JSON_HPP
