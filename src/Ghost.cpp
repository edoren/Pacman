#include "Ghost.hpp"

Ghost::Ghost(sf::Vector2f initialPos, std::string imgFile, int movementState, int housePos) : Sprite(imgFile), houseClock() {
    if (!frightenedTexture.loadFromFile(pacmanPath + "resources/images/frightenedGhost.png")) exit(EXIT_FAILURE);

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

    this->movAfterHouse = SCATTER_MOVE;
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
    
    this->frightenedFrame = 0;
    this->movementState = movementState;
    this->enableMovement = true;

    this->setTextureRect(sf::IntRect(20 * this->getFrame() + 3, 20 * this->getDirection() + 3, 14, 14));
}

int Ghost::getMovState() {
    if(this->movAfterHouse == FRIGHTENED_MOVE) return this->movAfterHouse;
    return this->movementState;
}

void Ghost::updateAnimation() {
    this->frameTimer = this->frameClock.getElapsedTime();
    if(movementState != FRIGHTENED_MOVE and movAfterHouse != FRIGHTENED_MOVE) {
        if(this->frameTimer.asSeconds() > 0.08f) {
            this->setTextureRect(sf::IntRect(20 * this->getFrame() + 3, 20 * this->getDirection() + 3, 14, 14));
            if (this->getFrame() == 0) this->setFrame(1);
            else this->setFrame(0);
            this->frameClock.restart();
        }
    } else {
        if(this->frightenedClock.getElapsedTime().asSeconds() < 3.5f) {
            if(this->frameTimer.asSeconds() > 0.08f) {
                this->setTextureRect(sf::IntRect(20 * this->getFrame() + 3, 3, 14, 14));
                if (this->getFrame() == 0) this->setFrame(1);
                else this->setFrame(0);
                this->frameClock.restart();
            }
        } else if((this->frightenedClock.getElapsedTime().asSeconds() >= 3.5f and this->frightenedClock.getElapsedTime().asSeconds() < 5.f)) {
            if(this->frameTimer.asSeconds() > 0.08f) {
                this->setTextureRect(sf::IntRect(20 * this->getFrame() + 3, 20 * this->frightenedFrame + 3, 14, 14));
                if (this->getFrame() == 0) this->setFrame(1);
                else {
                    this->setFrame(0);
                    if(this->frightenedFrame == 0) this->frightenedFrame = 1;
                    else this->frightenedFrame = 0;
                }
                this->frameClock.restart();
            }
        } else {
            this->setPosition((int)this->getPosition().x, (int)this->getPosition().y);
            this->setChase();
        }
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
    this->updateAnimation();

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
    this->updatePos();
}

void Ghost::stopMovement() {
    this->enableMovement = false;
}

void Ghost::restartHouseClock() {
    this->houseClock.restart();
}

void Ghost::setChase() {
    this->setTexture(this->texture);
    if(movementState == FRIGHTENED_MOVE) {
        this->setSpeed(this->getSpeed() * -2.f);
        this->movAfterHouse = SCATTER_MOVE;
    }
    this->movementState = CHASE_MOVE;
    this->invertSpriteDirection();
    this->setFrame(0);
    this->frameClock.restart();
}

void Ghost::setFrightened() {
    this->setTexture(frightenedTexture);
    if(movementState != FRIGHTENED_MOVE && movementState != HOUSE_MOVE) this->setSpeed(this->getSpeed() / -2.f);
    if(movementState != HOUSE_MOVE) {
        this->movementState = FRIGHTENED_MOVE;
        this->invertSpriteDirection();
    } else {
        this->movAfterHouse = FRIGHTENED_MOVE;
    }
    this->setFrame(0);
    this->frightenedFrame = 0;
    this->frightenedClock.restart();
    this->frameClock.restart();
}

void Ghost::frightenedMove(TileMap& map) {
    std::vector<sf::Vector2f> paths = this->avaliablePaths(map);
    if(!paths.empty()) {
        int randnum = rand() % paths.size();
        if(movementState != FRIGHTENED_MOVE) {
            if(paths[randnum] == VECTOR_UP) this->setSpriteDirection(SPRITE_UP);
            if(paths[randnum] == VECTOR_DOWN) this->setSpriteDirection(SPRITE_DOWN);
            if(paths[randnum] == VECTOR_LEFT) this->setSpriteDirection(SPRITE_LEFT);
            if(paths[randnum] == VECTOR_RIGHT) this->setSpriteDirection(SPRITE_RIGHT);
        }
        this->setSpeedDirection(paths[randnum]);
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
                    if(this->movAfterHouse == FRIGHTENED_MOVE) this->setSpeed(VECTOR_LEFT / 2.f);
                    else this->setSpeed(VECTOR_LEFT);
                           this->movementState = this->movAfterHouse;
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
                    if(this->movAfterHouse == FRIGHTENED_MOVE) this->setSpeed(VECTOR_RIGHT / 2.f);
                    else this->setSpeed(VECTOR_RIGHT);
                           this->movementState = this->movAfterHouse;
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
                    if(this->movAfterHouse == FRIGHTENED_MOVE) this->setSpeed(VECTOR_RIGHT / 2.f);
                    else this->setSpeed(VECTOR_RIGHT);
                           this->movementState = this->movAfterHouse;
                }
                break;
        }
    }
}

std::vector<sf::Vector2f> Ghost::avaliablePaths(TileMap& map) {
    std::vector<sf::Vector2f> avaliablePaths = map.avaliablePaths(this->getTilePos());
    auto iterator = std::find(avaliablePaths.begin(), avaliablePaths.end(), this->getSpeedDirection() * -1.f);
    if(iterator != avaliablePaths.end()) avaliablePaths.erase(iterator);
    return avaliablePaths;
}

