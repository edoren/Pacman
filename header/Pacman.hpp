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
    sf::Vector2f lastPos;
    
    sf::Clock clock;
    sf::Time timer;

public:
    Pacman();
    ~Pacman();

    bool getMouthState();
    void setMouthState(bool mouth);
    bool getAnimation();
    void setAnimation(bool animation);
    
    sf::Vector2f getLastPos();
    void setLastPos(sf::Vector2f lastPos);
    void setNextMovement(int keypressed);

    void startAnimation();
    void inputMovement(const sf::Sprite background);

private:
    void keyAction(sf::Vector2f direction, const sf::Sprite background, int spriteDirection);

};

#endif // __PACMAN_H__
