#ifndef GHOST_HPP
#define GHOST_HPP

#include "SFML/Graphics/Texture.hpp"
#include "STP/TMXLoader.hpp"

#include "Engine/AnimatorJson.hpp"

class Ghost : public AnimatorJson {
 public:
    enum Direction {
        Left = -2,
        Up = -1,
        Down = 1,
        Right = 2
    };

    enum State {
        Normal,
        Frightened,
        InHouse,
        OnlyEyes
    };

    enum Name {
        Blinky,
        Inky,
        Clyde,
        Pinky
    };

    Ghost(Name name, sf::Texture* ghost_texture, const std::string& working_dir);
    ~Ghost();

 private:
    sf::Vector2f velocity_;
    bool eatable_;
    Name name_;
    sf::Texture* ghost_texture_;
    std::string name_string_;
    Direction direction_;
    State state_;

    bool paused_;

 public:
    void pause();  // Pause the ghost movement and animation
    void resume();  // Resume the ghost movement and animation

    bool is_paused(); //

    void setDirection(Ghost::Direction direction);
    const Ghost::Direction& getDirection() const;

    void setState(Ghost::State state);
    Ghost::State getState();

    const sf::Vector2f& getVelocity() const;

    sf::FloatRect getCollisionBox();

    void updatePos();

    // Ghost movement types
    void randomMovement(tmx::TileMap *map);

 private:
    void changeAnimation(Ghost::State state);
};

#endif  // GHOST_HPP
