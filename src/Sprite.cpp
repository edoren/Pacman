#include "Sprite.hpp"

Sprite::Sprite() : speed(), direction(), frame() {}
Sprite::~Sprite() {}

sf::Vector2f Sprite::getSpeed() {
    return this->speed;
}

int Sprite::getFrame() {
    return this->frame;
}

int Sprite::getDirection() {
    return this->direction;
}

void Sprite::setSpeed(sf::Vector2f speed) {
    this->speed = speed;
}

void Sprite::setFrame(int frame){
    this->frame = frame;
}

void Sprite::setDirection(int direction) {
    this->direction = direction;
}
