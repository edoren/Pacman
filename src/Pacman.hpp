#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "Engine/AnimatorJson.hpp"

class Pacman : public AnimatorJson {
 public:
    Pacman(sf::Texture* pacman_texture);
    ~Pacman();

    enum Direction {
        Up,
        Left,
        Down,
        Right
    };

 private:
    bool is_alive_;
    Direction direction_;

 public:
    void setDirection(Pacman::Direction direction);

    void update_pos();

};

#endif  // PACMAN_HPP
