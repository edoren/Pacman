#include "Sprite.hpp"

Sprite::Sprite(std::string spriteImgFile) : speed(), speedDirection(), spriteDirection(), frame(), frameClock(), frameTimer() {
    if (!this->texture.loadFromFile(spriteImgFile)) exit(EXIT_FAILURE);
    this->setTexture(this->texture);
    this->setFrame(0);
    this->enableMovement = true;
    this->animation = true;
}

Sprite::~Sprite() {}

sf::Vector2f Sprite::getSpeed() {
    return this->speed;
}

sf::Vector2f Sprite::getSpeedDirection() {
    float magnitude = getVectMagnitude(this->speed);
    return sf::Vector2f(this->speed.x / magnitude, this->speed.y / magnitude);
}

int Sprite::getFrame() {
    return this->frame;
}

int Sprite::getDirection() {
    return this->spriteDirection;
}

sf::Vector2f Sprite::getTilePos() {
    return (this->getPosition() + sf::Vector2f(3, 3)) / TILE_SIZE;
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

void Sprite::setSpriteDirection(int spriteDirection) {
    this->spriteDirection = spriteDirection;
}

void Sprite::invertSpriteDirection() {
    switch(spriteDirection) {
        case SPRITE_UP:
            spriteDirection = SPRITE_DOWN;
            break;
        case SPRITE_DOWN:
            spriteDirection = SPRITE_UP;
            break;
        case SPRITE_RIGHT:
            spriteDirection = SPRITE_LEFT;
            break;
        case SPRITE_LEFT:
            spriteDirection = SPRITE_RIGHT;
            break;
    }
}

float Sprite::getVectMagnitude(sf::Vector2f vector) {
    return sqrtf( powf(vector.x, 2.0) + powf(vector.y, 2.0) );
}
