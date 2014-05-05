#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "Engine/AnimatorJson.hpp"

class Pacman : public AnimatorJson {
 public:
    Pacman(sf::Texture* pacman_texture, const std::string& working_dir);
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

    void updatePos();

};

#endif  // PACMAN_HPP
