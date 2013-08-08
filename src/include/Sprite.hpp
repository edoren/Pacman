#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <cmath>
#include <cassert>
// Headers for SFML2 modules
#include <SFML/Graphics.hpp>
// Collition and Config modules
#include "Collision.hpp"
#include "Config.hpp"

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
    
private:
    sf::Texture texture;

    sf::Vector2f speed;
    sf::Vector2f speedDirection; // Unitary vector
    int spriteDirection;
    int frame;

public:
    Sprite(std::string spriteImgFile);
    ~Sprite();

    sf::Vector2f getSpeed();
    sf::Vector2f setSpeedDirection();
    int getFrame();
    int getDirection();
    sf::Texture getSpriteTexture();

    void setSpeed(float speed);
    void setSpeed(sf::Vector2f speed);
    void setSpeedDirection(sf::Vector2f speedDirection);
    void setFrame(int frame);
    void setSpriteDirection(int spriteDirection);

private:
    float getVectMagnitude(sf::Vector2f vector);

};


#endif // __SPRITE_H__
