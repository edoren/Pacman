#ifndef GHOST_HPP
#define GHOST_HPP

#include "Sprite.hpp"
#include "TileMap.hpp"
#include "Collision.hpp"
#include <list>

#define CHASE_MOVE 0
#define SCATTER_MOVE 1
#define FRIGHTENED_MOVE 2
#define HOUSE_MOVE 3
#define TO_HOUSE 4

#define HOUSE_MIDDLE 0
#define HOUSE_LEFT 1
#define HOUSE_RIGHT 2

class Ghost : public Sprite {
private:
    sf::Texture frightenedTexture;
    sf::Texture eyesTexture;

    sf::Vector2f initialPos;
    std::string spriteImg;
    int movementState;

    sf::Vector2f lastPos;

    int frightenedFrame;
    sf::Clock chaseClock;
    sf::Clock scatterClock;
    sf::Clock frightenedClock;
    sf::Clock houseClock;
    float timeInHouse;
    int housePos;
    int movAfterHouse;

    std::list<sf::Vector2f> shortestDirection;
    int AIlevel;

public:
    Ghost(sf::Vector2f initialPos, std::string spriteImgFile, int movementState, int housePos, int AIlevel);

    int getMovState();

    void setFrightened();
    void setScatter();
    void setChase();
    void setHouseMove();

    void backToHouse(TileMap &map);

    void update(TileMap& map, sf::Vector2f pacmanTilePos, sf::Vector2f pacmanDirection);
    void restartHouseClock();
    void stopMovement();

private:
    std::vector<sf::Vector2f> avaliablePaths(TileMap& map);
    bool collisionInPoint(sf::Vector2f point, TileMap& map);

    void updateAnimation();
    void setSpriteDirectionSpeed(sf::Vector2f speed);
    void updatePos();

    // Movement algorithms
    void chaseMove(TileMap& map, sf::Vector2f pacmanTilePos, sf::Vector2f pacmanDirection);
    void scatterMove(TileMap& map);
    void frightenedMove(TileMap& map);
    void houseMove(TileMap& map, float time);
    void toHouseMove(TileMap& map);

    // Lua calls
    sf::Vector2f call_direction_chase(sf::Vector2f pacmanTilePos, sf::Vector2f pacmanDirection, std::vector<sf::Vector2f> paths);
    sf::Vector2f call_direction_scatter(std::vector<sf::Vector2f> paths);
    std::list<sf::Vector2f> call_Astar(sf::Vector2f actualPos, sf::Vector2f objetivePos, TileMap& map);
};

extern std::vector<Ghost*> ghosts;

#endif // GHOST_HPP
