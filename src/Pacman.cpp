#include "Pacman.hpp"

// Constructor
Pacman::Pacman() : Sprite(pacmanPath + "resources/images/pacmanSprites.png") {
    if (!this->loseTexture.loadFromFile(pacmanPath + "resources/images/pacmanLose.png")) exit(EXIT_FAILURE);
    this->setTextureRect(sf::IntRect(43, 3, 14, 14));
    this->setPosition(sf::Vector2f(105, 205));

    this->mouth = true;
    this->keypressed = KEYBOARD_NULL;

    this->setSpriteDirection(SPRITE_LEFT);
    this->setSpeed(sf::Vector2f(-1,0));
}

//Destructor
Pacman::~Pacman() {}

void Pacman::setNextMovement(int keypressed) {
    this->keypressed = keypressed;
}

// Animate pacman
void Pacman::updateAnimation() {
    if (this->animation){
        this->frameTimer = this->frameClock.getElapsedTime();
        if(this->frameTimer.asSeconds() > 0.04f)  {
            this->setTextureRect(sf::IntRect(20 * this->getFrame() + 3, 20 * this->getDirection() + 3, 14, 14));
            if (this->getFrame() == 0 && this->mouth){
                this->setFrame(1);
            } else if(this->getFrame() == 0 && !this->mouth) {
                this->setFrame(2);
            } else if(this->getFrame() == 1) {
                this->mouth = false;
                this->setFrame(0);
            } else {
                this->setTextureRect(sf::IntRect(43, 3, 14, 14));
                this->mouth = true;
                this->setFrame(0);
            }
            this->frameClock.restart();
        }
    }
}

// Move pacman
void Pacman::updatePos() {
    if (enableMovement) {
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
void Pacman::update(TileMap& map) {
    this->updateAnimation();
    this->updatePos();
    this->mapCollision(map);
}

void Pacman::keyAction(sf::Vector2f direction, TileMap &map, int spriteDirection) {
    if (!map.checkCollision(this->getTilePos(), spriteDirection)) {
        this->enableMovement = true;
        this->animation = true;
        this->setSpeedDirection(direction);
        this->setSpriteDirection(spriteDirection);
        this->keypressed = KEYBOARD_NULL;
    }
}

// Get the keyboard input
void Pacman::inputMovement(TileMap &map) {
    if(map.isValidTilePos(this->getTilePos()) == VALID_IN_RANGE) {
        if(this->keypressed != KEYBOARD_NULL) {
            switch(this->keypressed) {
                case KEYBOARD_UP:
                    Pacman::keyAction(VECTOR_UP, map, SPRITE_UP);
                    break;
                case KEYBOARD_DOWN:
                    Pacman::keyAction(VECTOR_DOWN, map, SPRITE_DOWN);
                    break;
                case KEYBOARD_LEFT:
                    Pacman::keyAction(VECTOR_LEFT, map, SPRITE_LEFT);
                    break;
                case KEYBOARD_RIGHT:
                    Pacman::keyAction(VECTOR_RIGHT, map, SPRITE_RIGHT);
                    break;
            }
        }
    }
}

// Backgound Collision
bool Pacman::mapCollision(TileMap &map) {
    if(map.checkCollision(this->getTilePos(), this->getDirection())){
        this->animation = false;
        this->enableMovement = false;
        return true;
    }
    return false;
}

bool Pacman::lose() {
    if(this->getFrame() == 12) return true;

    this->animation = false;
    this->enableMovement = false;

    this->setTexture(loseTexture);

    if(this->frameClock.getElapsedTime().asSeconds() > 0.1f)  {
        this->setTextureRect(sf::IntRect(20 * this->getFrame() + 3, 3, 14, 14));
        this->setFrame(this->getFrame()+1);
        this->frameClock.restart();
    }

    return false;
}
