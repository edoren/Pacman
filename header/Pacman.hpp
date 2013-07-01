#ifndef __PACMAN_H__
#define __PACMAN_H__

#include "Sprite.hpp"
#include "Input.hpp"

class Pacman : public Sprite {
private:
    sf::Texture texture;

    bool mouth; // true: pacman open the mouth, false: pacman close the mouth
    bool animation;
    int keypressed; // store the next movement of pacman
    
    sf::Clock clock;
    sf::Time timer;
    sf::Vector2f lastPos;

public:
    Pacman();
    ~Pacman();

    bool getMouthState();
    bool getAnimation();
    sf::Vector2f getLastPos();
    void setAnimation(bool animation);
    void setMouthState(bool mouth);
    void setLastPos(sf::Vector2f lastPos);
    void setNextMovement(int keypressed);

    void startAnimation();
    void inputMovement(const sf::Sprite background);
};

#endif // __PACMAN_H__
