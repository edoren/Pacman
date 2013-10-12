#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <cmath>
#include <cassert>
// Headers for SFML2 modules
#include <SFML/Graphics.hpp>
// Collition and Config modules
#include "Config.hpp"

#define TILE_SIZE 8.f

#define VECTOR_RIGHT sf::Vector2f(1, 0)
#define VECTOR_LEFT sf::Vector2f(-1, 0)
#define VECTOR_UP sf::Vector2f(0, -1)
#define VECTOR_DOWN sf::Vector2f(0, 1)

// Definitions for the SPRITE movement
#define SPRITE_UP 2
#define SPRITE_DOWN 3
#define SPRITE_RIGHT 1
#define SPRITE_LEFT 0

class Sprite : public sf::Sprite {
public:
    sf::Clock frameClock;
    sf::Time frameTimer;
    bool enableMovement;
    bool animation;

private:
    sf::Vector2f speed;
    sf::Vector2f speedDirection; // Unitary vector
    int spriteDirection;
    int frame;

public:
    Sprite(std::string spriteImgFile);
    ~Sprite();

    sf::Texture texture;
    sf::Vector2f getSpeed();
    sf::Vector2f getSpeedDirection();
    int getFrame();
    int getDirection();
    sf::Vector2f getTilePos();
    sf::Texture getSpriteTexture();

    void setSpeed(float speed);
    void setSpeed(sf::Vector2f speed);
    void setSpeedDirection(sf::Vector2f speedDirection);
    void setFrame(int frame);
    void setSpriteDirection(int spriteDirection);

    void invertSpriteDirection();

private:
    float getVectMagnitude(sf::Vector2f vector);

};

#endif // SPRITE_HPP
