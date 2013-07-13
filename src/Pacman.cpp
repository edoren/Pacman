#include "Pacman.hpp"

// Constructor
Pacman::Pacman() {
    if (!Collision::CreateTextureAndBitmask(this->texture, "resources/pacmanSprites.png")) exit(EXIT_FAILURE);
    this->setTexture(this->texture);
    this->setTextureRect(sf::IntRect(43, 3, 14, 14));
    this->setPosition(sf::Vector2f(95, 205));

    this->mouth = true;
    this->animation = true;
    this->enableMovement = true;
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

// Animate pacman
void Pacman::updateAnimation() {
    if (this->getAnimation()){
        this->frameTimer = this->frameClock.getElapsedTime();
        if(this->frameTimer.asSeconds() > 0.04f)  {
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
            this->frameClock.restart();
        }
    }
}

// Move pacman
void Pacman::updatePos() {
    if (enableMovement) {
        // Get the last position
        this->setLastPos(this->getPosition()); 
        // Check if pacman is out of position
        if(this->getPosition().x <= -12) {
            this->setPosition(sf::Vector2f(WINDOW_WIDTH-2, this->getPosition().y));
        } else if (this->getPosition().x >= WINDOW_WIDTH-1) {
            this->setPosition(sf::Vector2f(-11, this->getPosition().y));
        }
        // Set the new position
        this->setPosition(this->getPosition() + this->getSpeed());
    }
}

// Update all the pacman states
void Pacman::update() {
    this->updateAnimation();
    this->updatePos();
}

void Pacman::keyAction(sf::Vector2f direction, const sf::Sprite background, int spriteDirection) {
    this->enableMovement = true;
    this->setPosition(this->getPosition() + direction);
    if(!Collision::PixelPerfectTestOneObj(background, *this)){
        this->setSpeedDirection(direction);
        this->setDirection(spriteDirection);
        this->keypressed = KEYBOARD_NULL;
    }
    this->setPosition(this->getPosition() + direction * -1.f);
}

// Get the keyboard input
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

// Backgound Collision
bool Pacman::backgroundCollision(const sf::Sprite background) {
    if(Collision::PixelPerfectTestOneObj(background, *this)){
        this->setPosition(this->getLastPos());

        this->setAnimation(false);
        this->enableMovement = false;

        // std::cout << "Collision!!, Position: (" << pacman.getPosition().x << ", " << pacman.getPosition().y << ")" << std::endl;
        return true;
    }
    return false;
}
