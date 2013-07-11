#include "Sprite.hpp"

Sprite::Sprite() : speed(), speedDirection(), spriteDirection(), frame() {}
Sprite::~Sprite() {}

sf::Vector2f Sprite::getSpeed() {
    return this->speed;
}

int Sprite::getFrame() {
    return this->frame;
}

int Sprite::getDirection() {
    return this->spriteDirection;
}

void Sprite::setSpeed(sf::Vector2f speed) {
    this->speedDirection = speed / this->getVectMagnitude(speed);
    this->speed = speed;
}

// Conserve the same speed direction and just change the magnitude, the argument must be a magnitude
void Sprite::setSpeed(float speed) {
    this->speed = this->speedDirection * speed;
}

// Conserve the same speed magnitude and just change the direction, the argument must be a unitary vector
void Sprite::setSpeedDirection(sf::Vector2f speedDirection) {
    assert(this->getVectMagnitude(speedDirection) == 1);
    this->speedDirection = speedDirection;
    this->speed = speedDirection * this->getVectMagnitude(this->speed);
}

void Sprite::setFrame(int frame){
    this->frame = frame;
}

void Sprite::setDirection(int spriteDirection) {
    this->spriteDirection = spriteDirection;
}

float Sprite::getVectMagnitude(sf::Vector2f vector) {
    return sqrtf( powf(vector.x, 2.0) + powf(vector.y, 2.0) );
}
