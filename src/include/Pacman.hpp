#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "Collision.hpp"
#include "TileMap.hpp"
#include "Input.hpp"

class Pacman : public Sprite {    
private:
    bool mouth; // true: pacman open the mouth, false: pacman close the mouth
    sf::Texture loseTexture;

    int keypressed; // store the next movement of pacman

public:
    Pacman();
    ~Pacman();

    void setNextMovement(int keypressed);
    void inputMovement(TileMap &map);

    void update(TileMap &map);

    bool lose();

private:
    bool mapCollision(TileMap& map);

    void updateAnimation();
    void updatePos();

    void keyAction(sf::Vector2f direction, TileMap& map, int spriteDirection);
};

#endif // PACMAN_HPP
