#ifndef GHOST_HPP
#define GHOST_HPP

#include <map>

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
        ToHouse
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

    static std::map<Ghost::Name, Ghost*> ghosts;

private:
    Name name_;
    std::string name_string_;

    float speed_;

    Direction direction_;

    State state_;

    sf::Texture* ghost_texture_;

    bool paused_;

    enum HousePos {
        HouseLeft,
        HouseMiddle,
        HouseRight,
    };

    HousePos house_ubication_;

    sf::FloatRect house_bounds_;

 public:
    void pause();  // Pause the ghost movement and animation
    void resume();  // Resume the ghost movement and animation
    void restart();

    bool is_paused();

    void setDirection(Ghost::Direction direction);
    const Ghost::Direction& getDirection() const;

    void setState(Ghost::State state);
    const Ghost::State& getState();

    sf::FloatRect getCollisionBox();

    void updatePos(tmx::TileMap *map = nullptr, sf::Vector2f pacman_pos = {0,0}, Pacman::Direction pacman_dir = Pacman::None);

 private:
    void changeAnimation(Ghost::State state);

    // Ghost movement types
    void movementChooser(tmx::TileMap *map, const sf::Vector2f& pacman_pos, const Pacman::Direction& pacman_dir);
    void randomMovement(tmx::TileMap *map);
    void focusMovement(tmx::TileMap *map, const sf::Vector2f& target_pos);  // Move to a specific position
    void houseMovement();
    void toHouseMovement(tmx::TileMap *map, const sf::Vector2f& target_pos);
};

#endif  // GHOST_HPP
