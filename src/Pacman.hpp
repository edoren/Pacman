#ifndef __PACMAN_H__
#define __PACMAN_H__

#include "Sprite.hpp"

class Pacman : public Sprite {
private:
    sf::Texture texture;

    bool mouth; // true: pacman open the mouth, false: pacman close the mouth
    bool animation;
    sf::Clock clock;
    sf::Time timer;
    sf::Vector2f lastPos;

public:
    Pacman();
    ~Pacman();

    bool getMouthState();
    bool getAnimation();
    void setAnimation(bool animation);
    void setMouthState(bool mouth);
    void setLastPos(sf::Vector2f lastPos);
    sf::Vector2f getLastPos();

    void startAnimation();
};

#endif // __PACMAN_H__
