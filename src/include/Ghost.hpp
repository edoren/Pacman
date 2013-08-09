#define __GHOST_H__
#ifdef __GHOST_H__

#include "Sprite.hpp"

#define CHASE_MOVE 0
#define SCATTER_MOVE 1
#define FRIGHTENED_MOVE 2
#define HOUSE_MOVE 3

#define HOUSE_MIDDLE 0
#define HOUSE_LEFT 1
#define HOUSE_RIGHT 2

class Ghost : public Sprite {
private:
    sf::Vector2f initialPos;
    std::string spriteImg;
    int movementState;
    bool enableMovement;

    sf::Vector2f lastPos;

    sf::Clock houseClock;
    float timeInHouse;
    int housePos;

public:
    Ghost(sf::Vector2f initialPos, std::string spriteImgFile, int movementState, int housePos);

    void update(const sf::Sprite background);
    void stopMovement();

private:
    std::vector<sf::Vector2f> avaliablePaths(const sf::Sprite background);
    bool collisionInPoint(sf::Vector2f point, const sf::Sprite background);

    void updateAnimation();
    void updatePos();

    // Movement algorithms
    void randomMove(const sf::Sprite background);
    void houseMove(const sf::Sprite background, float time);
};

#endif // __GHOST_H__
