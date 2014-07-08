#include "Pacman.hpp"

Pacman::Pacman(sf::Texture* pacman_texture, const std::string& working_dir, const sf::Vector2f initial_pos)
      : initial_pos_(initial_pos),
        paused_(false),
        alive_(true) {
    this->loadJsonFile(working_dir + "assets/sprites/pacman/pacman.json", pacman_texture);
    restart();
}

Pacman::~Pacman() {};

void Pacman::setDirection(Pacman::Direction direction) {
	if (this->getCollisionBox().left < 0 || this->getCollisionBox().left >= 224) return;
	if (this->getCollisionBox().top < 0 || this->getCollisionBox().top >= 288) return;

    if (direction_ == direction) return;

    direction_ = direction;
    switch(direction_) {
        case Left:
            this->setAnimation("left");
            break;
        case Right:
            this->setAnimation("right");
            break;
        case Up:
            this->setAnimation("up");
            break;
        case Down:
            this->setAnimation("down");
            break;
        default:
            break;
    }
}

const Pacman::Direction& Pacman::getDirection() const {
    return direction_;
}

void Pacman::kill() {
    alive_ = false;
    this->setAnimation("death", false);
}

bool Pacman::isAlive() {
    return alive_;
}

const sf::Vector2f& Pacman::getVelocity() const {
    return velocity_;
}

sf::FloatRect Pacman::getCollisionBox() {
    sf::FloatRect g_bounds = this->getGlobalBounds();
    return sf::FloatRect(g_bounds.left + 3.f, g_bounds.top + 3.f, 8.f, 8.f);
}

void Pacman::updatePos() {
    if (!alive_ || paused_) return;

    switch(direction_) {
        case Left:
            velocity_ = sf::Vector2f(-1.f, 0);
            break;
        case Right:
            velocity_ = sf::Vector2f(1.f, 0);
            break;
        case Up:
            velocity_ = sf::Vector2f(0, -1.f);
            break;
        case Down:
            velocity_ = sf::Vector2f(0, 1.f);
            break;
        default:
            break;
    }

    this->move(velocity_);

	if (this->getPosition().x < -14) this->setPosition(224, this->getPosition().y);
	if (this->getPosition().x > 224) this->setPosition(-14, this->getPosition().y);

	if (this->getPosition().y < -14) this->setPosition(this->getPosition().x, 288);
	if (this->getPosition().y > 288) this->setPosition(this->getPosition().x, -14);
}

void Pacman::pause() {
    this->pauseAnimation();
    paused_ = true;
}

void Pacman::resume() {
    this->resumeAnimation();
    paused_ = false;
}

void Pacman::restart() {
    alive_ = true;
    paused_ = false;
    if (direction_ == Left) this->setAnimation("left");
    this->setDirection(Left);
    this->setPosition(initial_pos_);
};

bool Pacman::is_paused() {
    return paused_;
}
