#include "Pacman.hpp"

Pacman::Pacman(sf::Texture* pacman_texture, const std::string& working_dir) 
      : alive(true),
        paused_(false) {
    this->loadJsonFile(working_dir + "assets/sprites/pacman/pacman.json", pacman_texture);
    this->setDirection(Left);
}

Pacman::~Pacman() {};

void Pacman::setDirection(Pacman::Direction direction) {
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

const sf::Vector2f& Pacman::getVelocity() const {
    return velocity_;
}

sf::FloatRect Pacman::getCollisionBox() {
    sf::FloatRect g_bounds = this->getGlobalBounds();
    return sf::FloatRect(g_bounds.left + 3.f, g_bounds.top + 3.f, 8.f, 8.f);
}

void Pacman::updatePos() {
    if (!alive || paused_) return;

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
}

void Pacman::pause() {
    this->pauseAnimation();
    paused_ = true;
}

void Pacman::resume() {
    this->resumeAnimation();
    paused_ = false;
}

bool Pacman::is_paused() {
    return paused_;
}
