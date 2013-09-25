#ifndef GHOST_HPP
#define GHOST_HPP

#include "Sprite.hpp"
#include "TileMap.hpp"
#include "Collision.hpp"

#define CHASE_MOVE 0
#define SCATTER_MOVE 1
#define FRIGHTENED_MOVE 2
#define HOUSE_MOVE 3

#define HOUSE_MIDDLE 0
#define HOUSE_LEFT 1
#define HOUSE_RIGHT 2

class Ghost : public Sprite {
private:
    sf::Texture frightenedTexture;

    sf::Vector2f initialPos;
    std::string spriteImg;
    int movementState;
    bool enableMovement;

    sf::Vector2f lastPos;

    sf::Clock frightenedClock;
    int frightenedFrame;
    sf::Clock *scatterClock;
    sf::Clock houseClock;
    float timeInHouse;
    int housePos;
    int movAfterHouse;

public:
    Ghost(sf::Vector2f initialPos, std::string spriteImgFile, int movementState, int housePos);

    int getMovState();

    void setFrightened();
    void setScatter();
    void setChase();

    void update(TileMap& map);
    void restartHouseClock();
    void stopMovement();

private:
    std::vector<sf::Vector2f> avaliablePaths(TileMap& map);
    bool collisionInPoint(sf::Vector2f point, TileMap& map);

    void updateAnimation();
    void updatePos();

    // Movement algorithms
    void frightenedMove(TileMap& map);
    void houseMove(TileMap& map, float time);
};

#endif // GHOST_HPP
