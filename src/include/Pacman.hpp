#ifndef __PACMAN_H__
#define __PACMAN_H__

#include "Sprite.hpp"
#include "Input.hpp"

class Pacman : public Sprite {
private:
    bool mouth; // true: pacman open the mouth, false: pacman close the mouth
    bool animation;
    sf::Texture loseTexture;
    bool enableMovement; // if true pacman moves
    
    int keypressed; // store the next movement of pacman
    sf::Vector2f lastPos;

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

    void update(const sf::Sprite background);
    void inputMovement(const sf::Sprite background);

    bool lose();

private:
    bool backgroundCollision(const sf::Sprite background);
    
    void updateAnimation();
    void updatePos();

    void keyAction(sf::Vector2f direction, const sf::Sprite background, int spriteDirection);

};

#endif // __PACMAN_H__
