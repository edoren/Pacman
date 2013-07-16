#define __GHOST_H__
#ifdef __GHOST_H__

#include "Sprite.hpp"

class Ghost : public Sprite {
private:
    sf::Texture texture;

    sf::Vector2f initialPos;
    std::string spriteImg;
    int movementState;

    sf::Vector2f lastPos;

    std::vector<sf::Vector2f> test;

public:
    Ghost(sf::Vector2f initialPos, std::string spriteImgFile, int spriteDirection);

    void update(const sf::Sprite background);


private:
    std::vector<sf::Vector2f> avaliablePaths(const sf::Sprite background);
    bool collisionInPoint(sf::Vector2f point, const sf::Sprite background);

    void updateAnimation();
    void updatePos();

    // Movement algorithms
    void randomMove(const sf::Sprite background);
};

#endif // __GHOST_H__
