#include "Pacman.hpp"

Pacman::Pacman(sf::Texture* pacman_texture) : is_alive_(true) {
    this->load_json_file("assets/sprites/pacman/pacman.json", pacman_texture);
    this->setDirection(Left);
}

Pacman::~Pacman() {};

void Pacman::setDirection(Pacman::Direction direction) {
    direction_ = direction;
    switch(direction_) {
        case Left:
            this->set_animation("left");
            break;
        case Right:
            this->set_animation("right");
            break;
        case Up:
            this->set_animation("up");
            break;
        case Down:
            this->set_animation("down");
            break;
    }
}

void Pacman::update_pos() {
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
