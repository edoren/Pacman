#ifndef __SPRITE_H__
#define __SPRITE_H__

// Headers for SFML2 modules
#include <SFML/Graphics.hpp>

// Collition module
#include "Collision.hpp"

// Definitions for the SPRITE movement
#define SPRITE_UP 2
#define SPRITE_DOWN 3
#define SPRITE_RIGHT 1
#define SPRITE_LEFT 0

class Sprite : public sf::Sprite {
private:
    sf::Vector2f speed;
    int direction;
    int frame;

public:
    Sprite();
    ~Sprite();

    sf::Vector2f getSpeed();
    int getFrame();
    int getDirection();
    sf::Texture getSpriteTexture();


    void setSpeed(sf::Vector2f speed);
    void setFrame(int frame);
    void setDirection(int direction);
};


#endif // __SPRITE_H__
