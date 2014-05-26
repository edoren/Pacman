#include <cmath>
#include <limits>

#include "Ghost.hpp"
#include "Collision.hpp"

Ghost::Ghost(Name name, sf::Texture* ghost_texture, const std::string& working_dir) :
        eatable_(false),
        name_(name),
        state_(Ghost::State::Chase),
        paused_(false) {
    ghost_texture_ = ghost_texture;
    this->loadJsonFile(working_dir + "assets/sprites/ghosts/ghosts.json", ghost_texture_);

    switch (name_) {
        case Blinky:
            name_string_ = "blinky";
            this->setDirection(Left);
            break;
        case Inky:
            name_string_ = "inky";
            break;
        case Clyde:
            name_string_ = "clyde";
            break;
        case Pinky:
            name_string_ = "pinky";
            break;
        default:
            break;
    }
}

Ghost::~Ghost() {}

void Ghost::setDirection(Ghost::Direction direction) {
    if (direction_ == direction) return;

    direction_ = direction;
    this->changeAnimation(state_);
}

const Ghost::Direction& Ghost::getDirection() const {
    return direction_;
}

const sf::Vector2f& Ghost::getVelocity() const {
    return velocity_;
}

sf::FloatRect Ghost::getCollisionBox() {
    sf::FloatRect g_bounds = this->getGlobalBounds();
    return sf::FloatRect(g_bounds.left + 3.f, g_bounds.top + 3.f, 8.f, 8.f);
}

void Ghost::setState(Ghost::State state) {
    state_ = state;
    this->changeAnimation(state);
}

Ghost::State Ghost::getState() {
    return state_;
}

void Ghost::updatePos() {
    if (paused_) return;

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

void Ghost::changeAnimation(Ghost::State state) {
    const char* animation_name;
    switch (state) {
        case Chase:
        case Scatter:
        case InHouse:
            animation_name = name_string_.c_str();
            break;
        case Frightened:
            this->setAnimation("frightened");
            return;
            break;
        case OnlyEyes:
            animation_name = "eyes";
            break;
    }

    char up[15], down[15], left[15], right[15];

    sprintf(up, "%s-up", animation_name);
    sprintf(down, "%s-down", animation_name);
    sprintf(left, "%s-left", animation_name);
    sprintf(right, "%s-right", animation_name);

    switch (direction_) {
        case Left:
            this->setAnimation(left);
            break;
        case Right:
            this->setAnimation(right);
            break;
        case Up:
            this->setAnimation(up);
            break;
        case Down:
            this->setAnimation(down);
            break;
        default:
            break;
    }
}

void Ghost::pause() {
    this->pauseAnimation();
    paused_ = true;
}

void Ghost::resume() {
    this->resumeAnimation();
    paused_ = false;
}

bool Ghost::is_paused() {
    return paused_;
}

void Ghost::randomMovement(tmx::TileMap *map) {
    // Positions to check
    std::vector<std::pair<Ghost::Direction, sf::Vector2f>> postocheck{
        {Ghost::Direction::Left, {-1.f, 0}},
        {Ghost::Direction::Right, {1.f, 0}},
        {Ghost::Direction::Up, {0, -1.f}},
        {Ghost::Direction::Down,  {0, 1.f}}
    };

    std::vector<Ghost::Direction> posible_paths;

    for (const auto &pos : postocheck) {
        if (pos.first != -this->getDirection()) {
            // Move the ghost to the wanted direction to check collision with the map
            this->move(pos.second);

            // Check if no exist collision with the map
            if (!Collision::checkMapCollision(map, this->getCollisionBox())) {
                posible_paths.push_back(pos.first);
            }

            // Return the ghost to last position
            this->move(pos.second * -1.f);
        }
    }

    if (posible_paths.size() == 0) return;

    int rand_index = rand() % static_cast<unsigned int>(posible_paths.size());
    this->setDirection(posible_paths[rand_index]);
}

void Ghost::focusMovement(tmx::TileMap *map, sf::Vector2f position) {
    // Positions to check
    std::vector<std::pair<Ghost::Direction, sf::Vector2f>> postocheck{
        {Ghost::Direction::Left, {-1.f, 0}},
        {Ghost::Direction::Right, {1.f, 0}},
        {Ghost::Direction::Up, {0, -1.f}},
        {Ghost::Direction::Down,  {0, 1.f}}
    };

    float short_distance = std::numeric_limits<float>::max();
    Ghost::Direction next_direction = Ghost::Direction::None;

    for (const auto &pos : postocheck) {
        if (pos.first != -this->getDirection()) {
            // Move the ghost to the wanted direction to check collision with the map
            this->move(pos.second);

            // Check if no exist collision with the map
            if (!Collision::checkMapCollision(map, this->getCollisionBox())) {
                float x_delta = position.x - this->getPosition().x;
                float y_delta = position.y - this->getPosition().y;
                float distance = hypot(x_delta, y_delta);
                if (distance < short_distance) {
                    short_distance = distance;
                    next_direction = pos.first;
                }
            }

            // Return the ghost to last position
            this->move(pos.second * -1.f);
        }
    }

    if (next_direction != Ghost::Direction::None)
        this->setDirection(next_direction);
}
