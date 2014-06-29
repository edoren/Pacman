#include <cmath>
#include <limits>

#include "Ghost.hpp"
#include "Collision.hpp"

std::map<Ghost::Name, Ghost*> Ghost::ghosts;

Ghost::Ghost(Name name, sf::Texture* ghost_texture, const sf::FloatRect& house_bounds, const std::string& working_dir)
      : state_timer(false),
        name_(name),
        speed_(1.f),
        paused_(false),
        house_bounds_(house_bounds) {
    ghost_texture_ = ghost_texture;
    this->loadJsonFile(working_dir + "assets/sprites/ghosts/ghosts.json", ghost_texture_);

    auto initial_values = [&] (const char* name, const Ghost::HousePos& house_ubication, const Ghost::State& state,
                               const Ghost::Direction initial_dir, const sf::Vector2f& initial_pos) {
        name_string_ = name;
        house_ubication_ = house_ubication;
        this->setState(state);
        this->setDirection(initial_dir);
        this->setPosition(initial_pos);
    };

    switch (name_) {
        case Blinky:
            initial_values("blinky", HouseMiddle, Scatter, Right, {house_bounds.left + (house_bounds.width / 2) - 7, house_bounds.top - 8 - 3});
            break;
        case Inky:
            initial_values("inky", HouseLeft, InHouse, Up, {house_bounds.left + (house_bounds.width / 4) - 7, house_bounds.top + (house_bounds.height / 2) - 7});
            break;
        case Clyde:
            initial_values("clyde", HouseRight, InHouse, Up, {house_bounds.left + (house_bounds.width * 3/4) - 7, house_bounds.top + (house_bounds.height / 2) - 7});
            break;
        case Pinky:
            initial_values("pinky", HouseMiddle, InHouse, Down, {house_bounds.left + (house_bounds.width / 2) - 7, house_bounds.top + (house_bounds.height / 2) - 7});
            break;
    }

    // Add the ghost to the ghost map
    ghosts[name_] = this;
}

Ghost::~Ghost() {
    ghosts.erase(name_);
}

void Ghost::setDirection(Ghost::Direction direction) {
    if (direction_ == direction) return;

    direction_ = direction;
    this->changeAnimation(state_);
}

const Ghost::Direction& Ghost::getDirection() const {
    return direction_;
}

sf::FloatRect Ghost::getCollisionBox() {
    sf::FloatRect g_bounds = this->getGlobalBounds();
    return sf::FloatRect(g_bounds.left + 3.f, g_bounds.top + 3.f, 8.f, 8.f);
}

void Ghost::setState(Ghost::State state) {
    if (state == Frightened && (state_ == InHouse || state_ == ToHouse)) return;

    if (state_ == state) {
        if (state == Frightened) state_timer.restart();
        else return;
    }

    if (state == Frightened || state == InHouse) {
        speed_ = 0.5;  // Slow down ghost movement in a half
    } else if (state == ToHouse) {
        sf::Vector2f fixed_pos = {
            ceilf(this->getPosition().x),
            ceilf(this->getPosition().y)
        };

        if ((static_cast<int>(fixed_pos.x) & 0x1) == 0)
            fixed_pos.x = fixed_pos.x - 1;

        if ((static_cast<int>(fixed_pos.y) & 0x1) == 0)
            fixed_pos.y = fixed_pos.y - 1;

        this->setPosition(fixed_pos);

        speed_ = 2.0;
    } else {
        speed_ = 1.0; // Normal speed movement
    }

    if (state == Frightened) direction_ = static_cast<Direction>(-direction_);

    state_ = state;
    this->changeAnimation(state);
    state_timer.restart();
}

const Ghost::State& Ghost::getState() {
    return state_;
}

void Ghost::updatePos(tmx::TileMap *map, sf::Vector2f pacman_pos, Pacman::Direction pacman_dir) {
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

    if (map) this->movementChooser(map, pacman_pos, pacman_dir);
}

void Ghost::changeAnimation(Ghost::State state) {
    const char* animation_name;
    switch (state) {
        case Chase:
        case Scatter:
            animation_name = name_string_.c_str();
            break;
        case InHouse:
            animation_name = name_string_.c_str();
            break;
        case Frightened:
            this->setAnimation("frightened");
            return;
            break;
        case ToHouse:
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

void Ghost::movementChooser(tmx::TileMap *map, const sf::Vector2f& pacman_pos, const Pacman::Direction& pacman_dir) {
    sf::Vector2f target;

    if (state_ == State::InHouse) {
        this->houseMovement();
        return;
    } else if (state_ == State::ToHouse) {
        this->toHouseMovement(map, {(house_bounds_.left + 28), (house_bounds_.top - 8)});
        return;
    }

    // Check if the ghost is not on a Tile
    if ((static_cast<int>(this->getCollisionBox().left) % 8) != 0 ||
        (static_cast<int>(this->getCollisionBox().top) % 8) != 0) {
        return;
    }

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
            this->randomMovement(map);
            if (state_timer.getElapsedTime().asSeconds() >= 5) {
                this->setState(Chase);
                const sf::Vector2f& pos = this->getPosition();
                this->setPosition(floorf(pos.x), floorf(pos.y));
            }
            break;
        default:
            break;
    }
}

void Ghost::randomMovement(tmx::TileMap *map) {
    // Positions to check
    std::vector<std::pair<Ghost::Direction, sf::Vector2f>> postocheck{
        {Ghost::Direction::Left, {-speed_, 0}},
        {Ghost::Direction::Right, {speed_, 0}},
        {Ghost::Direction::Up, {0, -speed_}},
        {Ghost::Direction::Down,  {0, speed_}}
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
            this->move(-pos.second);
        }
    }

    if (posible_paths.size() == 0) return;

    int rand_index = rand() % static_cast<unsigned int>(posible_paths.size());
    this->setDirection(posible_paths[rand_index]);
}

void Ghost::focusMovement(tmx::TileMap *map, const sf::Vector2f& target_pos) {
    // Positions to check
    std::vector<std::pair<Ghost::Direction, sf::Vector2f>> postocheck{
        {Ghost::Direction::Left, {-speed_, 0}},
        {Ghost::Direction::Right, {speed_, 0}},
        {Ghost::Direction::Up, {0, -speed_}},
        {Ghost::Direction::Down,  {0, speed_}}
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
            this->move(-pos.second);
        }
    }

    if (next_direction != Ghost::Direction::None)
        this->setDirection(next_direction);
}

void Ghost::houseMovement() {
    if (this->getPosition().y <= house_bounds_.top + 9 ||
        this->getPosition().y >= house_bounds_.top + house_bounds_.height - 9 - 14 )
        this->setDirection(static_cast<Direction>(-direction_));

    auto movement = [&] (float time_in_house, const Ghost::Direction& leaving, const Ghost::Direction& out) {
        // In the respective house position
        if (state_timer.getElapsedTime().asSeconds() >= time_in_house) {
            // If is in the center of the house
            if (this->getPosition().x == house_bounds_.left + (house_bounds_.width / 2) - 7) {
                // If is out the house
                if (this->getPosition().y == house_bounds_.top - 8 - 3) {
                    this->setDirection(out);
                    this->setState(Scatter);
                    return;
                }
                this->setDirection(Up);
                return;
            }
            this->setDirection(leaving);
        }
    };

    switch (house_ubication_) {
        case HouseLeft:
            movement(4.f, Right, Right);
            break;
        case HouseRight:
            movement(6.f, Left, Left);
            break;
        case HouseMiddle:
            movement(2.f, Up, Left);
            break;
    }
}

void Ghost::toHouseMovement(tmx::TileMap *map, const sf::Vector2f& target_pos) {
    sf::Vector2f ghost_house_pos;
    sf::Vector2f house_middle = {house_bounds_.left + (house_bounds_.width / 2) - 7,
                                 house_bounds_.top + (house_bounds_.height / 2) - 7};
    sf::Vector2f house_outside = {house_bounds_.left + (house_bounds_.width / 2) - 7,
                                  house_bounds_.top - 8 - 3};

    auto movement = [&] (const Ghost::Direction& arrive, const Ghost::Direction& inside) {
        // Check if the ghost is in the respective house position
        if (this->getPosition() == ghost_house_pos) {
            this->setDirection(arrive);
            this->setState(InHouse);
        }
        // Check if the ghost is in the middle of the house
        else if (this->getPosition() == house_middle) {
            this->setDirection(inside);
        }
        // Check if the ghost is outside the house
        else if (this->getPosition() == house_outside) {
            this->setDirection(Down);
        }
    };

    switch (house_ubication_) {
        case HouseRight:
            ghost_house_pos = {house_bounds_.left + (house_bounds_.width * 3/4) - 7,
                               house_bounds_.top + (house_bounds_.height / 2) - 7};
            movement(Up, Right);
            break;
        case HouseLeft:
            ghost_house_pos = {house_bounds_.left + (house_bounds_.width / 4) - 7,
                               house_bounds_.top + (house_bounds_.height / 2) - 7};
            movement(Up, Left);
            break;
        case HouseMiddle:
            ghost_house_pos = house_middle;
            movement(Down, None);
            break;
    }

    // Check if the ghost is inside the house
    if (Collision::AABBCollision(house_bounds_, this->getCollisionBox())) return;

    // Check if the ghost is not on a Tile
    if ((static_cast<int>(this->getCollisionBox().left) % 8) != 0 ||
        (static_cast<int>(this->getCollisionBox().top) % 8) != 0) {
        return;
    }

    this->focusMovement(map, target_pos);
}
