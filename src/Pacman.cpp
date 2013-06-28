#include "Pacman.hpp"

// Constructor
Pacman::Pacman() : lastPos(), timer(), clock() {
    Collision::CreateTextureAndBitmask(this->texture, "resources/pacmanSprites.png");
    this->setTexture(this->texture);
    this->setTextureRect(sf::IntRect(43, 3, 14, 14));
    this->setPosition(sf::Vector2f(95, 205));

    this->mouth = true;
    this->setDirection(SPRITE_LEFT);
    this->animation = true;
    this->setFrame(0);
    this->setSpeed(sf::Vector2f(-1,0));
}

//Destructor
Pacman::~Pacman() {}


sf::Vector2f Pacman::getLastPos() {
    return this->lastPos;
}

bool Pacman::getAnimation() {
    return this->animation;
}

bool Pacman::getMouthState() {
    return this->mouth;
}

void Pacman::setMouthState(bool mouth) {
    this->mouth = mouth;
}

void Pacman::setLastPos(sf::Vector2f lastPos) {
    this->lastPos = lastPos;
}

void Pacman::setAnimation(bool animation) {
    this->animation = animation;
}

void Pacman::startAnimation() {
    // Animate pacman
    if (this->getAnimation()){
        timer = this->clock.getElapsedTime();
        if(timer.asSeconds() > 0.04f)  {
            this->setTextureRect(sf::IntRect(20*this->getFrame()+3, 20*this->getDirection()+3, 14, 14));
            if (this->getFrame() == 0 && this->getMouthState()){
                this->setFrame(1);
            } else if(this->getFrame() == 0 && !this->getMouthState()) {
                this->setFrame(2);
            } else if(this->getFrame() == 1) {
                this->setMouthState(false);
                this->setFrame(0);
            } else {
                this->setTextureRect(sf::IntRect(43, 3, 14, 14));
                this->setMouthState(true);
                this->setFrame(0);
            }
            this->clock.restart();
        }
    }
}
