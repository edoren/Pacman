#ifndef GHOST_HPP
#define GHOST_HPP

#include "SFML/Graphics/Texture.hpp"
#include "STP/TMXLoader.hpp"

#include "Engine/AnimatorJson.hpp"
#include "Pacman.hpp"

class Ghost : public AnimatorJson {
 public:
    enum Direction {
        Left = -2,
        Up = -1,
        None = 0,
        Down = 1,
        Right = 2
    };

    enum State {
        Chase,
        Scatter,
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

    Ghost(Name name, sf::Texture* ghost_texture, const sf::FloatRect& house_bounds, const std::string& working_dir);
    ~Ghost();

    // Measure the time of the current animation - Starts stopped
    Clock state_timer;

private:
    Name name_;
    std::string name_string_;

    bool eatable_;

    float speed_;
    Direction direction_;
    sf::Vector2f position_;

    State state_;

    sf::Texture* ghost_texture_;

    bool paused_;

    enum {
        HouseLeft,
        HouseMiddle,
        HouseRight,
        HouseNone
    } house_ubication_;

    sf::FloatRect house_bounds_;

 public:
    void pause();  // Pause the ghost movement and animation
    void resume();  // Resume the ghost movement and animation

    bool is_paused(); //

    void setDirection(Ghost::Direction direction);
    const Ghost::Direction& getDirection() const;

    void setState(Ghost::State state);
    Ghost::State getState();

    float getSpeed() const;

    void setPosition(const sf::Vector2f& pos);
    void move(const sf::Vector2f& offset);

    sf::FloatRect getCollisionBox();

    void updatePos();

    void movementChooser(tmx::TileMap *map, sf::Vector2f pacman_pos, Pacman::Direction pacman_dir);

 private:
    void changeAnimation(Ghost::State state);

    // Ghost movement types
    void randomMovement(tmx::TileMap *map);
    void focusMovement(tmx::TileMap *map, sf::Vector2f target_pos);  // Move to a specific position
    void houseMovement();
};

#endif  // GHOST_HPP
