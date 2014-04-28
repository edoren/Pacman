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
    
    int load_json_string(const char* json_string, sf::Texture* texture = nullptr);
    int load_json_string(const std::string& json_string, sf::Texture* texture = nullptr);

    int load_json_file(const char* json_file, sf::Texture* texture = nullptr);
    int load_json_file(const std::string& json_file, sf::Texture* texture = nullptr);

    int set_animation(const char* animation_name);
    int set_animation(const std::string& animation_name);

    void set_frame(int index);

    void update_animation();

    void pause_animation();
    void resume_animation();

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

    int load_json_type(json_t *data, sf::Texture* texture = nullptr);
};

#endif  // ANIMATOR_JSON_HPP
