#include <cmath>
#include <limits>

#include "Ghost.hpp"
#include "Collision.hpp"

Ghost::Ghost(Name name, sf::Texture* ghost_texture, const sf::FloatRect& house_bounds, const std::string& working_dir)
      : state_timer(false),
        name_(name),
        eatable_(false),
        paused_(false),
        house_bounds_(house_bounds) {
    ghost_texture_ = ghost_texture;
    this->loadJsonFile(working_dir + "assets/sprites/ghosts/ghosts.json", ghost_texture_);

    switch (name_) {
        case Blinky:
            name_string_ = "blinky";
            state_ = Scatter;
            house_ubication_ = HouseNone;
            this->setDirection(Direction::Right);
            this->setPosition({house_bounds.left + (house_bounds.width / 2) - 7,
                               house_bounds.top - 8 - 3});
            break;
        case Inky:
            name_string_ = "inky";
            house_ubication_ = HouseLeft;
            state_ = InHouse;
            this->setDirection(Direction::Up);
            this->setPosition({house_bounds.left + (house_bounds.width / 4) - 7,
                               house_bounds.top + (house_bounds.height / 2) - 7});
            break;
        case Clyde:
            name_string_ = "clyde";
            house_ubication_ = HouseRight;
            state_ = InHouse;
            this->setDirection(Direction::Up);
            this->setPosition({house_bounds.left + (house_bounds.width * 3/4) - 7,
                               house_bounds.top + (house_bounds.height / 2) - 7});
            break;
        case Pinky:
            name_string_ = "pinky";
            house_ubication_ = HouseMiddle;
            state_ = InHouse;
            this->setDirection(Direction::Down);
            this->setPosition({house_bounds.left + (house_bounds.width / 2) - 7,
                               house_bounds.top + (house_bounds.height / 2) - 7});
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

float Ghost::getSpeed() const {
    return speed_;
}

void Ghost::setPosition(const sf::Vector2f& pos) {
    position_ = pos;
    this->AnimatorJson::setPosition({floorf(position_.x), floorf(position_.y)});
}

void Ghost::move(const sf::Vector2f& offset) {
    position_ += offset;
    this->AnimatorJson::setPosition({floorf(position_.x), floorf(position_.y)});
}

sf::FloatRect Ghost::getCollisionBox() {
    sf::FloatRect g_bounds = this->getGlobalBounds();
    return sf::FloatRect(g_bounds.left + 3.f, g_bounds.top + 3.f, 8.f, 8.f);
}

void Ghost::setState(Ghost::State state) {
    if (state_ == state) return;

    state_ = state;
    this->changeAnimation(state);
    state_timer.restart();
}

Ghost::State Ghost::getState() {
    return state_;
}

void Ghost::updatePos() {
    if (paused_) return;

    sf::Vector2f velocity;

    switch(direction_) {
        case Left:
            velocity = sf::Vector2f(-speed_, 0);
            break;
        case Right:
            velocity = sf::Vector2f(speed_, 0);
            break;
        case Up:
            velocity = sf::Vector2f(0, -speed_);
            break;
        case Down:
            velocity = sf::Vector2f(0, speed_);
            break;
        default:
            break;
    }

    this->move(velocity);
}

void Ghost::changeAnimation(Ghost::State state) {
    const char* animation_name;
    switch (state) {
        case Chase:
        case Scatter:
            animation_name = name_string_.c_str();
            speed_ = 1.f;
            break;
        case InHouse:
            animation_name = name_string_.c_str();
            speed_ = 0.5f;
            break;
        case Frightened:
            this->setAnimation("frightened");
            speed_ = -0.5f;
            return;
            break;
        case OnlyEyes:
            animation_name = "eyes";
            speed_ = 0.5f;
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

void Ghost::movementChooser(tmx::TileMap *map, sf::Vector2f pacman_pos, Pacman::Direction pacman_dir) {
    if (state_ == State::InHouse) {
        this->houseMovement();
        return;
    }

    // Check if the ghost is not on a Tile
    if ((static_cast<int>(this->getCollisionBox().left) % 8) != 0 ||
        (static_cast<int>(this->getCollisionBox().top) % 8) != 0) {
        return;
    }

    sf::Vector2f target;
    sf::Vector2f pacman_facing;  // Pacman's facing unitary vector

    sf::Vector2f distance_to_pacman;

    switch(pacman_dir) {
        case Left:
            pacman_facing = {-1, 0};
            break;
        case Right:
            pacman_facing = {1, 0};
            break;
        case Up:
            pacman_facing = {0, -1};
            break;
        case Down:
            pacman_facing = {0, 1};
            break;
        default:
            break;
    }

    switch (state_) {
        case Ghost::Chase:
            switch (name_) {
                case Blinky:
                    target = pacman_pos;
                    break;
                case Inky:
                    // Target the pacman position but four tiles ahead.
                    distance_to_pacman = pacman_pos + pacman_facing * (2.f * 8.f) - this->getPosition();
                    target = pacman_pos + distance_to_pacman;
                    break;
                case Pinky:
                    // Target the pacman position but four tiles ahead.
                    distance_to_pacman = pacman_pos + pacman_facing * (2.f * 8.f) - this->getPosition();
                    target = pacman_pos + pacman_facing * (4.f * 8.f);
                    break;
                case Clyde:
                    distance_to_pacman = pacman_pos - this->getPosition();
                    if ((abs(distance_to_pacman.x) + abs(distance_to_pacman.y)) / 8 <= 8)
                        target = {0*8, 35*8};
                    else
                        target = pacman_pos;
                    break;
            }
            this->focusMovement(map, target);
            break;
        case Ghost::Scatter:
            switch (name_) {
                case Blinky:
                    target = {25*8, 0*8};
                    break;
                case Inky:
                    target = {27*8, 35*8};
                    break;
                case Pinky:
                    target = {2*8, 0*8};
                    break;
                case Clyde:
                    target = {0*8, 35*8};
                    break;
            }
            if (state_timer.getElapsedTime().asSeconds() >= 7) this->setState(Chase);
            this->focusMovement(map, target);
            break;
        case Ghost::Frightened:
            break;
        case Ghost::OnlyEyes:
            break;
        default:
            break;
    }
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

void Ghost::focusMovement(tmx::TileMap *map, sf::Vector2f target_pos) {
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
                float x_delta = target_pos.x - this->getPosition().x;
                float y_delta = target_pos.y - this->getPosition().y;
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

void Ghost::houseMovement() {
    if (this->getPosition().y <= house_bounds_.top + 9 ||
        this->getPosition().y >= house_bounds_.top + house_bounds_.height - 9 - 14 )
        this->setDirection(static_cast<Direction>(-direction_));

    switch (house_ubication_) {
        case HouseLeft:
            if (state_timer.getElapsedTime().asSeconds() >= 4.f) {
                // If is in the center of the house
                if (this->getPosition().x == house_bounds_.left + (house_bounds_.width / 2) - 7) {
                    // If is out the house
                    if (this->getPosition().y == house_bounds_.top - 8 - 3) {
                        this->setDirection(Right);
                        this->setState(Scatter);
                        return;
                    }
                    this->setDirection(Up);
                    return;
                }
                this->setDirection(Right);
            }
            break;
        case HouseRight:
            if (state_timer.getElapsedTime().asSeconds() >= 6.f) {
                // If is in the center of the house
                if (this->getPosition().x == house_bounds_.left + (house_bounds_.width / 2) - 7) {
                    // If is out the house
                    if (this->getPosition().y == house_bounds_.top - 8 - 3) {
                        this->setDirection(Left);
                        this->setState(Scatter);
                        return;
                    }
                    this->setDirection(Up);
                    return;
                }
                this->setDirection(Left);
            }
            break;
        case HouseNone:
        case HouseMiddle:
            if (state_timer.getElapsedTime().asSeconds() >= 2.f) {
                // If is in the center of the house
                if (this->getPosition().x == house_bounds_.left + (house_bounds_.width / 2) - 7) {
                    // If is out the house
                    if (this->getPosition().y == house_bounds_.top - 8 - 3) {
                        this->setDirection(Left);
                        this->setState(Scatter);
                        return;
                    }
                    this->setDirection(Up);
                    return;
                }
            }
            break;
    }
}
