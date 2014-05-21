#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "Engine/AnimatorJson.hpp"

class Pacman : public AnimatorJson {
 public:
    Pacman(sf::Texture* pacman_texture, const std::string& working_dir);
    ~Pacman();

    enum Direction {
        Left = -2,
        Up = -1,
        None = 0,
        Down = 1,
        Right = 2
    };

    bool alive;

    void pause();  // Pause pacman movement and animation
    void resume();  // Resume pacman movement and animation

    bool is_paused();

 private:
    Direction direction_;
    sf::Vector2f velocity_;
    bool paused_;

 public:
    void setDirection(Pacman::Direction direction);
    const Pacman::Direction& getDirection() const;

    const sf::Vector2f& getVelocity() const;

    sf::FloatRect getCollisionBox();

    void updatePos();
};

#endif  // PACMAN_HPP
