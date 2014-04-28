#ifndef GHOST_HPP
#define GHOST_HPP

#include "SFML/Graphics/Texture.hpp"

#include "Engine/AnimatorJson.hpp"

class Ghost : public AnimatorJson {
 public:
    enum Direction {
        Up,
        Left,
        Down,
        Right,
        Stopped
    };

    enum State {
        Normal,
        Frightened,
        OnlyEyes
    };

    enum Name {
        Blinky,
        Inky,
        Clyde,
        Pinky
    };

    Ghost(Name name, sf::Texture* ghost_texture);
    ~Ghost();

 private:
    float speed_;
    bool eatable_;
    Name name_;
    sf::Texture* ghost_texture_;
    std::string name_string_;
    Direction direction_;
    State state_;

 public:
    void setDirection(Ghost::Direction direction);
    void setState(Ghost::State state);
    void update_pos();

 private:
    void changeAnimation(Ghost::State state);
};

#endif  // GHOST_HPP
