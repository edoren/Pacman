#include "Ghost.hpp"

Ghost::Ghost(sf::Vector2f initialPos, std::string imgFile, int movementState, int housePos) : Sprite(imgFile), houseClock() {
    this->setPosition(initialPos);
    this->lastPos = this->getPosition();

    switch(movementState) {
        case SCATTER_MOVE:
            this->setSpriteDirection(SPRITE_LEFT);
            this->setSpeed(VECTOR_LEFT);
            break;
        case HOUSE_MOVE:
            if(housePos == HOUSE_MIDDLE) {
                this->setSpriteDirection(SPRITE_DOWN);
                this->setSpeed(VECTOR_DOWN / 2.f);
            } else {
                this->setSpriteDirection(SPRITE_UP);
                this->setSpeed(VECTOR_UP / 2.f);
            }
            break;
    }

    this->housePos = housePos;
    switch(housePos) {
        case HOUSE_MIDDLE:
            this->timeInHouse = 2.f;
            break;
        case HOUSE_LEFT:
            this->timeInHouse = 4.f;
            break;
        case HOUSE_RIGHT:
            this->timeInHouse = 6.f;
            break;
    }
    
    this->movementState = movementState;
    this->enableMovement = true;

    this->setTextureRect(sf::IntRect(20 * this->getFrame() + 3, 20 * this->getDirection() + 3, 14, 14));
}

void Ghost::updateAnimation() {
    this->frameTimer = this->frameClock.getElapsedTime();
    if(movementState != FRIGHTENED_MOVE) {
            if(this->frameTimer.asSeconds() > 0.08f)  {
                this->setTextureRect(sf::IntRect(20 * this->getFrame() + 3, 20 * this->getDirection() + 3, 14, 14));
                if (this->getFrame() == 0){
                    this->setFrame(1);
                } else if(this->getFrame() == 1) {
                    this->setFrame(0);
                }
            this->frameClock.restart();
        }
    } else {
        
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

void Ghost::update(TileMap& map) {
    if(map.isValidTilePos(this->getTilePos()) == VALID_IN_RANGE) {
        if(this->enableMovement) {
            switch(this->movementState) {
                case CHASE_MOVE:
                    this->frightenedMove(map);
                    break;
                case SCATTER_MOVE:
                    this->frightenedMove(map);
                    break;
                case FRIGHTENED_MOVE:
                    this->frightenedMove(map);
                    break;
            }
        }
    }
    if (this->movementState == HOUSE_MOVE) {
        this->houseMove(map, 2.f);
    }
    this->updateAnimation();
    this->updatePos();
}

void Ghost::stopMovement() {
    this->enableMovement = false;
}

void Ghost::restartHouseClock() {
    this->houseClock.restart();
}

void Ghost::setFrightened() {
    movementState = FRIGHTENED_MOVE;
    this->setFrame(0);
}

void Ghost::frightenedMove(TileMap& map) {
    std::vector<sf::Vector2f> paths = this->avaliablePaths(map);
    if(!paths.empty()) {
        int randnum = rand() % paths.size();
        if(paths[randnum] == VECTOR_UP) this->setSpriteDirection(SPRITE_UP);
        if(paths[randnum] == VECTOR_DOWN) this->setSpriteDirection(SPRITE_DOWN);
        if(paths[randnum] == VECTOR_LEFT) this->setSpriteDirection(SPRITE_LEFT);
        if(paths[randnum] == VECTOR_RIGHT) this->setSpriteDirection(SPRITE_RIGHT);
        this->setSpeed(paths[randnum]);
    }
}

void Ghost::houseMove(TileMap& map, float time) {
    sf::Vector2f tilePos = sf::Vector2f(std::ceil(getTilePos().x), getTilePos().y);
    if(this->houseClock.getElapsedTime().asSeconds() < this->timeInHouse) {
        if(map.checkCollision(tilePos - sf::Vector2f(0, 3/8.f), SPRITE_UP)) {
            this->setSpeedDirection(VECTOR_DOWN);
            this->setSpriteDirection(SPRITE_DOWN);
        }
        if(map.checkCollision(tilePos + sf::Vector2f(0, 3/8.f), SPRITE_DOWN)) {
            this->setSpeedDirection(VECTOR_UP);
            this->setSpriteDirection(SPRITE_UP);
        }
    } else {
        switch(this->housePos) {
            case HOUSE_MIDDLE:
                if(this->getPosition().x == 105 and this->getPosition().y > 109) {
                    this->setSpeedDirection(VECTOR_UP);
                } else {
                    this->setSpeed(VECTOR_LEFT);
                    this->movementState = SCATTER_MOVE;
                }
                break;
            case HOUSE_LEFT:
                if(this->getPosition().x < 105) {
                    this->setSpriteDirection(SPRITE_RIGHT);
                    this->setSpeedDirection(VECTOR_RIGHT);
                } else if(this->getPosition().x == 105 and this->getPosition().y > 109) {
                    this->setSpriteDirection(SPRITE_UP);
                    this->setSpeedDirection(VECTOR_UP);
                } else {
                    this->setSpeed(VECTOR_RIGHT);
                    this->movementState = SCATTER_MOVE;
                }
                break;
            case HOUSE_RIGHT:
                if(this->getPosition().x > 105) {
                    this->setSpriteDirection(SPRITE_LEFT);
                    this->setSpeedDirection(VECTOR_LEFT);
                } else if(this->getPosition().x == 105 and this->getPosition().y > 109) {
                    this->setSpriteDirection(SPRITE_UP);
                    this->setSpeedDirection(VECTOR_UP);
                } else {
                    this->setSpeed(VECTOR_RIGHT);
                    this->movementState = SCATTER_MOVE;
                }
                break;
        }
    }
}

std::vector<sf::Vector2f> Ghost::avaliablePaths(TileMap& map) {
    std::vector<sf::Vector2f> avaliablePaths = map.avaliablePaths(this->getTilePos());
    auto iterator = std::find(avaliablePaths.begin(), avaliablePaths.end(), this->getSpeed() * -1.f);
    if(iterator != avaliablePaths.end()) avaliablePaths.erase(iterator);
    return avaliablePaths;
}

