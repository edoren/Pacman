////////////////////////////////
// Headers                    //
////////////////////////////////

#include <string>

#include "jansson.h"
#include "SFML/Graphics.hpp"

#include "Engine/AnimatorJson.hpp"


////////////////////////////////
// Constructors               //
////////////////////////////////

AnimatorJson::AnimatorJson() :
        rel_path_(),
        external_texture_(false),
        sprite_texture_(nullptr),
        frame_index_(0),
        frame_duration_(0),
        clock_(),
        root_(nullptr),
        animation_(nullptr) {
}

////////////////////////////////
// Destructor                 //
////////////////////////////////

AnimatorJson::~AnimatorJson() {
    json_decref(root_);

    if (!external_texture_)
        delete sprite_texture_;
}

////////////////////////////////
// Public members             //
////////////////////////////////

int AnimatorJson::loadJsonString(const char* json_string, sf::Texture* texture) {
    json_t *data;
    json_error_t error;

    data = json_loads(json_string, 0, &error);

    if (!data) {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return 0;
    }

    int status = this->loadJsonType(data, texture);

    return status;
}

int AnimatorJson::loadJsonString(const std::string& json_string, sf::Texture* texture) {
    return this->loadJsonString(json_string.c_str(), texture);
}

int AnimatorJson::loadJsonFile(const char* json_file, sf::Texture* texture) {
    json_t *data;
    json_error_t error;

    std::string json_file_string(json_file);
    rel_path_ = json_file_string.substr(0, json_file_string.rfind('/') + 1);

    data = json_load_file(json_file, 0, &error);

    if (!data) {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return 0;
    }

    int status = this->loadJsonType(data, texture);

    return status;
}

int AnimatorJson::loadJsonFile(const std::string& json_file, sf::Texture* texture) {
    return this->loadJsonFile(json_file.c_str(), texture);
}

int AnimatorJson::setAnimation(const std::string& animation_name) {
    return this->setAnimation(animation_name.c_str());
}

int AnimatorJson::setAnimation(const char* animation_name) {
    if (!root_) {
        fprintf(stderr, "error: first load the json data.\n");
        return 0;
    }

    animation_ = json_object_get(root_, animation_name);

    if (!json_is_array(animation_)) {
        fprintf(stderr, "error: can not find animation %s.\n", animation_name);
        return 0;
    }

    this->setFrame(0);

    return 1;
}

void AnimatorJson::setFrame(int index) {
    json_t *frame, *frame_data;

    int frame_width, frame_height;
    int frame_xpos, frame_ypos;

    frame_index_ = index;
    frame = json_array_get(animation_, index);

    if (!json_is_object(frame)) {
        fprintf(stderr, "error: index %i out of range.\n", frame_index_);
        return;
    }

    frame_duration_ = json_integer_value(json_object_get(frame, "duration"));

    frame_data = json_object_get(frame, "frame");
    frame_width = json_integer_value(json_object_get(frame_data, "w"));
    frame_height = json_integer_value(json_object_get(frame_data, "h"));
    frame_xpos = json_integer_value(json_object_get(frame_data, "x"));
    frame_ypos = json_integer_value(json_object_get(frame_data, "y"));

    this->setTextureRect(sf::IntRect(frame_xpos, frame_ypos, frame_width, frame_height));

    clock_.restart();
}

void AnimatorJson::updateAnimation() {
    if (!animation_) return;
    if (clock_.getElapsedTime().asMilliseconds() >= frame_duration_) {
        this->setFrame((frame_index_ + 1) % json_array_size(animation_));
    }
}

void AnimatorJson::pauseAnimation() {
    clock_.pause();
}

void AnimatorJson::resumeAnimation() {
    clock_.resume();
}

////////////////////////////////
// Private members            //
////////////////////////////////

int AnimatorJson::loadJsonType(json_t *data, sf::Texture* texture) {
    root_ = data;

    // Delete local texture before assign a new one
    if (!external_texture_)
        delete sprite_texture_;

    if (texture != nullptr) {
        external_texture_ = true;
        sprite_texture_ = texture;
        this->setTexture(*sprite_texture_);
        return 1;
    }

    json_t *meta, *image;
    std::string image_file;

    if (!json_is_object(root_)) {
        fprintf(stderr, "error: JSON data is not an object.\n");
        json_decref(root_);
        return 0;
    }

    meta = json_object_get(root_, "meta");

    if (!json_is_object(meta)) {
        fprintf(stderr, "error: meta data is not an object.\n");
        json_decref(root_);
        return 0;
    }

    image = json_object_get(meta, "image");

    if (!json_is_string(image)) {
        fprintf(stderr, "error: image value is not a string.\n");
        json_decref(root_);
        return 0;
    }
    
    external_texture_ = false;
    image_file = json_string_value(image);
    sprite_texture_ = new sf::Texture();
    sprite_texture_->loadFromFile(rel_path_ + image_file);
    this->setTexture(*sprite_texture_);

    return 1;
}
