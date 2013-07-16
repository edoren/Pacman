#include "Ghost.hpp"

Ghost::Ghost(sf::Vector2f initialPos, std::string imgFile, int spriteDirection) : Sprite(imgFile) {
    this->setPosition(initialPos);

    this->lastPos = this->getPosition();

    this->setDirection(spriteDirection);
    this->setFrame(0);
    this->setSpeed(sf::Vector2f(-1,0));

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
    this->lastPos = this->getPosition();
    if(this->getPosition().x <= -12) {
        this->setPosition(sf::Vector2f(WINDOW_WIDTH-2, this->getPosition().y));
    } else if (this->getPosition().x >= WINDOW_WIDTH-1) {
        this->setPosition(sf::Vector2f(-11, this->getPosition().y));
    }
    this->setPosition(this->getPosition() + this->getSpeed());
}

void Ghost::update(const sf::Sprite background) {
    this->randomMove(background);
    this->updateAnimation();
    this->updatePos();
}

void Ghost::randomMove(const sf::Sprite background) {
    std::vector<sf::Vector2f> paths = this->avaliablePaths(background);
    if(!paths.empty()) {
        int randnum = rand() % paths.size()+1;
        if(paths[randnum-1] == VECTOR_UP) this->setDirection(SPRITE_UP);
        if(paths[randnum-1] == VECTOR_DOWN) this->setDirection(SPRITE_DOWN);
        if(paths[randnum-1] == VECTOR_LEFT) this->setDirection(SPRITE_LEFT);
        if(paths[randnum-1] == VECTOR_RIGHT) this->setDirection(SPRITE_RIGHT);
        this->setSpeed(paths[randnum-1]);
    }
}

std::vector<sf::Vector2f> Ghost::avaliablePaths(const sf::Sprite background) {
    std::vector<sf::Vector2f> avaliablePaths;
    // Check for all the posibles paths in the actual position
    if(this->collisionInPoint(VECTOR_UP, background) && this->getSpeed() != VECTOR_DOWN) avaliablePaths.push_back(VECTOR_UP);
    if(this->collisionInPoint(VECTOR_DOWN, background) && this->getSpeed() != VECTOR_UP) avaliablePaths.push_back(VECTOR_DOWN);
    if(this->collisionInPoint(VECTOR_LEFT, background) && this->getSpeed() != VECTOR_RIGHT) avaliablePaths.push_back(VECTOR_LEFT);
    if(this->collisionInPoint(VECTOR_RIGHT, background) && this->getSpeed() != VECTOR_LEFT) avaliablePaths.push_back(VECTOR_RIGHT);
    return avaliablePaths;
}

bool Ghost::collisionInPoint(sf::Vector2f point, const sf::Sprite background) {
    this->setPosition(this->getPosition() + point);
    if(Collision::PixelPerfectTestOneObj(background, *this)) {
        this->setPosition(this->getPosition() - point);
        return false;
    }
    this->setPosition(this->getPosition() - point);
    return true;
}
