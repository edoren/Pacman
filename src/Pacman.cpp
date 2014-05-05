#include "Pacman.hpp"

Pacman::Pacman(sf::Texture* pacman_texture, const std::string& working_dir) : is_alive_(true) {
    this->loadJsonFile(working_dir + "assets/sprites/pacman/pacman.json", pacman_texture);
    this->setDirection(Left);
}

Pacman::~Pacman() {};

void Pacman::setDirection(Pacman::Direction direction) {
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
    }
}

void Pacman::updatePos() {
    switch(direction_) {
        case Left:
            this->move(sf::Vector2f(-1.f, 0));
            break;
        case Right:
            this->move(sf::Vector2f(1.f, 0));
            break;
        case Up:
            this->move(sf::Vector2f(0, -1.f));
            break;
        case Down:
            this->move(sf::Vector2f(0, 1.f));
            break;
    }
}
