#include "Pacman.hpp"

// Constructor
Pacman::Pacman() : clock(), timer() {
    Collision::CreateTextureAndBitmask(this->texture, "resources/pacmanSprites.png");
    this->setTexture(this->texture);
    this->setTextureRect(sf::IntRect(43, 3, 14, 14));
    this->setPosition(sf::Vector2f(95, 205));

    this->mouth = true;
    this->animation = true;
    this->keypressed = KEYBOARD_NULL;
    this->lastPos = this->getPosition();

    this->setDirection(SPRITE_LEFT);
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

void Pacman::setNextMovement(int keypressed) {
    this->keypressed = keypressed;
}

void Pacman::startAnimation() {
    // Animate pacman
    if (this->getAnimation()){
        timer = this->clock.getElapsedTime();
        if(timer.asSeconds() > 0.04f)  {
            this->setTextureRect(sf::IntRect(20 * this->getFrame() + 3, 20 * this->getDirection() + 3, 14, 14));
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

void Pacman::keyAction(sf::Vector2f direction, const sf::Sprite background, int spriteDirection) {
    this->setPosition(this->getPosition() + direction);
    if(!Collision::PixelPerfectTest(background, *this)){
        this->setSpeed(direction);
        this->setDirection(spriteDirection);
        this->keypressed = KEYBOARD_NULL;
    }
    this->setPosition(this->getPosition() + direction * -1.f);
}

void Pacman::inputMovement(const sf::Sprite background) {
    if(this->keypressed != KEYBOARD_NULL) {
        switch(this->keypressed) {
            case KEYBOARD_UP: 
                Pacman::keyAction(sf::Vector2f(0,-1), background, SPRITE_UP);
                break;
            case KEYBOARD_DOWN:
                Pacman::keyAction(sf::Vector2f(0, 1), background, SPRITE_DOWN);
                break;
            case KEYBOARD_LEFT:
                Pacman::keyAction(sf::Vector2f(-1, 0), background, SPRITE_LEFT);
                break;
            case KEYBOARD_RIGHT:
                Pacman::keyAction(sf::Vector2f(1, 0), background, SPRITE_RIGHT);
                break;
        }
        this->setAnimation(true);
    }
}

bool Pacman::backgroundCollision(const sf::Sprite background) {
    // Backgound Collision
    if(Collision::PixelPerfectTest(background, *this)){
        this->setPosition(this->getLastPos());

        this->setAnimation(false);
        this->setSpeed(sf::Vector2f(0, 0));

        // std::cout << "Collision!!, Position: (" << pacman.getPosition().x << ", " << pacman.getPosition().y << ")" << std::endl;
        return true;
    }
    return false;
}
