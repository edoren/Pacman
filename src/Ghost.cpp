#include "Ghost.hpp"

Ghost::Ghost(Name name, sf::Texture* ghost_texture, const std::string& working_dir) :
        speed_(1.f),
        eatable_(false),
        name_(name),
        state_(Normal) {
    ghost_texture_ = ghost_texture;
    this->loadJsonFile(working_dir + "assets/sprites/ghosts/ghosts.json", ghost_texture_);

    switch (name_) {
        case Blinky:
            name_string_ = "blinky";
            this->setDirection(Left);
            break;
        case Inky:
            name_string_ = "inky";
            break;
        case Clyde:
            name_string_ = "clyde";
            break;
        case Pinky:
            name_string_ = "pinky";
            break; 
    }   
}

Ghost::~Ghost() {}

void Ghost::setDirection(Ghost::Direction direction) {
    direction_ = direction;
    this->changeAnimation(state_);
}

void Ghost::setState(Ghost::State state) {
    state_ = state;
    this->changeAnimation(state);
}

void Ghost::updatePos() {
    switch (direction_) {
        case Left:
            this->move(sf::Vector2f(-speed_, 0));
            break;
        case Right:
            this->move(sf::Vector2f(speed_, 0));
            break;
        case Up:
            this->move(sf::Vector2f(0, -speed_));
            break;
        case Down:
            this->move(sf::Vector2f(0, speed_));
            break;
        case Stopped:
            break;
    }
}

void Ghost::changeAnimation(Ghost::State state) {
    const char* animation_name;
    switch (state) {
        case Normal:
            animation_name = name_string_.c_str();
            break;
        case Frightened:
            this->setAnimation("frightened");
            return;
            break;
        case OnlyEyes:
            animation_name = "eyes";
            break;
    }

    char up[15], down[15], left[15], right[15]; 

    sprintf(up, "%s-up", animation_name);
    sprintf(down, "%s-down", animation_name);
    sprintf(left, "%s-left", animation_name);
    sprintf(right, "%s-right", animation_name);

    switch (direction_) {
        case Left:
            this->setAnimation(left);
            break;
        case Right:
            this->setAnimation(right);
            break;
        case Up:
            this->setAnimation(up);
            break;
        case Down:
            this->setAnimation(down);
            break;
    }
}
