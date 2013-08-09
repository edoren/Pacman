#include "Ghost.hpp"

Ghost::Ghost(sf::Vector2f initialPos, std::string imgFile, int movementState, int housePos) : Sprite(imgFile), houseClock() {
    this->setPosition(initialPos);
    this->lastPos = this->getPosition();

    switch(movementState) {
        case CHASE_MOVE:
            this->setSpriteDirection(SPRITE_LEFT);
            this->setSpeed(VECTOR_LEFT);
            break;
        case HOUSE_MOVE:
            this->setSpriteDirection(SPRITE_DOWN);
            this->setSpeed(VECTOR_DOWN / 2.f);
            break;
    }

    this->housePos = housePos;
    switch(housePos) {
        case HOUSE_MIDDLE:
            this->timeInHouse = 1.f;
            break;
        case HOUSE_LEFT:
            this->timeInHouse = 3.f;
            break;
        case HOUSE_RIGHT:
            this->timeInHouse = 5.f;
            break;
    }
    
    this->movementState = movementState;
    this->enableMovement = true;

    this->setTextureRect(sf::IntRect(20 * this->getFrame() + 3, 20 * this->getDirection() + 3, 14, 14));
}

void Ghost::updateAnimation() {
    this->frameTimer = this->frameClock.getElapsedTime();
        if(this->frameTimer.asSeconds() > 0.08f)  {
            this->setTextureRect(sf::IntRect(20 * this->getFrame() + 3, 20 * this->getDirection() + 3, 14, 14));
            if (this->getFrame() == 0){
                this->setFrame(1);
            } else if(this->getFrame() == 1) {
                this->setFrame(0);
            }
        this->frameClock.restart();
    }
}

void Ghost::updatePos() {
    if (this->enableMovement) {
        this->lastPos = this->getPosition();
        if(this->getPosition().x <= -12) {
            this->setPosition(sf::Vector2f(WINDOW_WIDTH-2, this->getPosition().y));
        } else if (this->getPosition().x >= WINDOW_WIDTH-1) {
            this->setPosition(sf::Vector2f(-11, this->getPosition().y));
        }
        this->setPosition(this->getPosition() + this->getSpeed());
    }
}

void Ghost::update(const sf::Sprite background) {
    if(this->enableMovement) {
        switch(this->movementState) {
            case SCATTER_MOVE:
                this->randomMove(background);
                break;
            case HOUSE_MOVE:
                this->houseMove(background, 2.f);
                break;
        }
    }
    this->updateAnimation();
    this->updatePos();
}

void Ghost::stopMovement() {
    this->enableMovement = false;
}

void Ghost::randomMove(const sf::Sprite background) {
    std::vector<sf::Vector2f> paths = this->avaliablePaths(background);
    if(!paths.empty()) {
        int randnum = rand() % paths.size();
        if(paths[randnum] == VECTOR_UP) this->setSpriteDirection(SPRITE_UP);
        if(paths[randnum] == VECTOR_DOWN) this->setSpriteDirection(SPRITE_DOWN);
        if(paths[randnum] == VECTOR_LEFT) this->setSpriteDirection(SPRITE_LEFT);
        if(paths[randnum] == VECTOR_RIGHT) this->setSpriteDirection(SPRITE_RIGHT);
        this->setSpeed(paths[randnum]);
    }
}

void Ghost::houseMove(const sf::Sprite background, float time) {
    if(this->houseClock.getElapsedTime().asSeconds() < this->timeInHouse) {
        if(this->collisionInPoint(VECTOR_UP, background)) {
            this->setSpeedDirection(VECTOR_DOWN);
            this->setSpriteDirection(SPRITE_DOWN);
        }
        if(this->collisionInPoint(VECTOR_DOWN, background)) {
            this->setSpeedDirection(VECTOR_UP);
            this->setSpriteDirection(SPRITE_UP);
        }
    } else {
        switch(this->housePos) {
            case HOUSE_MIDDLE:
                if(this->getPosition().x == 105 and this->getPosition().y > 125) {
                    this->setSpeedDirection(VECTOR_UP);
                } else {
                    this->setSpeed(VECTOR_UP);
                    this->movementState = SCATTER_MOVE;
                }
                break;
            case HOUSE_LEFT:
                if(this->getPosition().x < 105) {
                    this->setSpriteDirection(SPRITE_RIGHT);
                    this->setSpeedDirection(VECTOR_RIGHT);
                } else if(this->getPosition().x == 105 and this->getPosition().y > 125) {
                    this->setSpriteDirection(SPRITE_UP);
                    this->setSpeedDirection(VECTOR_UP);
                } else {
                    this->setSpeed(VECTOR_UP);
                    this->movementState = SCATTER_MOVE;
                }
                break;
            case HOUSE_RIGHT:
                if(this->getPosition().x > 105) {
                    this->setSpriteDirection(SPRITE_LEFT);
                    this->setSpeedDirection(VECTOR_LEFT);
                } else if(this->getPosition().x == 105 and this->getPosition().y > 125) {
                    this->setSpriteDirection(SPRITE_UP);
                    this->setSpeedDirection(VECTOR_UP);
                } else {
                    this->setSpeed(VECTOR_UP);
                    this->movementState = SCATTER_MOVE;
                }
                break;
        }
    }
}

std::vector<sf::Vector2f> Ghost::avaliablePaths(const sf::Sprite background) {
    std::vector<sf::Vector2f> avaliablePaths;
    // Check for all the posibles paths in the actual position
    if(!this->collisionInPoint(VECTOR_UP, background) and this->getSpeed() != VECTOR_DOWN) avaliablePaths.push_back(VECTOR_UP);
    if(!this->collisionInPoint(VECTOR_DOWN, background) and this->getSpeed() != VECTOR_UP and this->getPosition() != sf::Vector2f(105, 109)) avaliablePaths.push_back(VECTOR_DOWN);
    if(!this->collisionInPoint(VECTOR_LEFT, background) and this->getSpeed() != VECTOR_RIGHT) avaliablePaths.push_back(VECTOR_LEFT);
    if(!this->collisionInPoint(VECTOR_RIGHT, background) and this->getSpeed() != VECTOR_LEFT) avaliablePaths.push_back(VECTOR_RIGHT);
    return avaliablePaths;
}

bool Ghost::collisionInPoint(sf::Vector2f point, const sf::Sprite background) {
    this->setPosition(this->getPosition() + point);
    if(!Collision::PixelPerfectTestOneObj(background, *this)) {
        this->setPosition(this->getPosition() - point);
        return false;
    }
    this->setPosition(this->getPosition() - point);
    return true;
}
