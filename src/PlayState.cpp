#include <cstdlib>

#include <vector>
#include <utility>

#include "PlayState.hpp"

PlayState PlayState::PlayState_;

PlayState::PlayState() {}

void PlayState::init(ResourceManager* resources, Settings* settings) {
    // Pacman and Ghosts texture is always loaded while the game is open
    sf::Texture* pacman_texture = resources->loadTexture("assets/sprites/pacman/pacman.png");
    sf::Texture* ghosts_texture = resources->loadTexture("assets/sprites/ghosts/ghosts.png");

    const std::string& working_dir = resources->getWorkingDirectory();

    map_ = new tmx::TileMap(working_dir + "assets/maps/clasic.tmx");
    pacman_ = new Pacman(pacman_texture, working_dir);

    pacman_->setPosition({112 - 3, 208 - 3});

    blinky_ = new Ghost(Ghost::Name::Blinky, ghosts_texture, working_dir);
    blinky_->setDirection(Ghost::Direction::Right);

    blinky_->setPosition({112 - 3, 208 - 3});

    // Get the settings
    int width = settings->getSetting("width", 224);
    int height = settings->getSetting("height", 288);
    int volume = settings->getSetting("volume", 100);
};

void PlayState::exit(ResourceManager* resources) {
    delete pacman_;
    delete blinky_;
    delete map_;
};

void PlayState::pause() {
}; 

void PlayState::resume() {
};

void PlayState::handleEvents(GameEngine* game) {
    sf::RenderWindow* window = game->getWindow();
    sf::Event event;
    while (window->pollEvent(event)) {
        // Close window : exit
        if (event.type == sf::Event::Closed ||
            (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) ||
            (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::F4)))
            game->quit();
        // Manage the pacman next movement
        if (event.type == sf::Event::KeyPressed) {
            switch(event.key.code) {
                case sf::Keyboard::Up:
                    next_dir_ = Pacman::Direction::Up;
                    break;
                case sf::Keyboard::Down:
                    next_dir_ = Pacman::Direction::Down;
                    break;
                case sf::Keyboard::Left:
                    next_dir_ = Pacman::Direction::Left;
                    break;
                case sf::Keyboard::Right:
                    next_dir_ = Pacman::Direction::Right;
                    break;
            }
        }
    }
}

void PlayState::frameStarted(GameEngine* game) {
    this->updatePacman();
    this->updateGhost(blinky_);
}

void PlayState::frameEnded(GameEngine* game) {
}

void PlayState::draw(GameEngine* game) {
    sf::RenderWindow* window = game->getWindow();
    window->draw(*map_);
    window->draw(*blinky_);
    window->draw(*pacman_);
}

bool PlayState::checkMapCollision(const sf::FloatRect& collision_box) {
    tmx::Layer& meta_tiles = map_->GetLayer("MetaTiles");

    // Iterate over all the MetaTiles
    for (unsigned int y = 0; y < meta_tiles.GetHeight(); ++y) {
        for (unsigned int x = 0; x < meta_tiles.GetWidth(); ++x) {

            tmx::Layer::Tile& tile = meta_tiles.GetTile(x, y);

            // Check if the tile is not an empty tile and has the collapsible property in t(true),
            // if it has, checks the collision and return true if it happens
            if (!tile.empty() && tile.GetPropertyValue("collapsible") == "t" && 
                this->AABBCollision(collision_box, tile.GetGlobalBounds())) {
                return true;
            }
        }
    }

    return false;
}

void PlayState::updatePacman() {
    // Update pacman position and animation
    pacman_->updatePos();
    pacman_->updateAnimation();

    // Check pacman collision with the food
    this->checkFoodCollision();

    // Check if pacman is not on a Tile, if not it checks if he want to go backwards
    if ((static_cast<int>(pacman_->getCollisionBox().left) % 8) != 0 || 
        (static_cast<int>(pacman_->getCollisionBox().top) % 8) != 0) {
        if (pacman_->getDirection() == -next_dir_) {
            pacman_->setDirection(next_dir_);
            next_dir_ = Pacman::Direction::None;
        }
        return;
    }

    // Check if the front tile is collapsible
    if (pacman_->getDirection() == next_dir_ || next_dir_ == Pacman::Direction::None) {
        // Move pacman to the front
        pacman_->move(pacman_->getVelocity());

        // Check if exist collision with the map
        if (this->checkMapCollision(pacman_->getCollisionBox())) {
            pacman_->pause(); // Pause pacman movement and animation
        }

        // Return pacman to last position
        pacman_->move(pacman_->getVelocity() * -1.f);

        return;
    }

    // Resume pacman if is paused
    if (pacman_->is_paused()) pacman_->resume();

    sf::Vector2f move_dir;

    // Select the move offset according to the pacman's direction
    switch(next_dir_) {
        case Pacman::Direction::Left:
            move_dir = sf::Vector2f(-1.f, 0);
            break;
        case Pacman::Direction::Right:
            move_dir = sf::Vector2f(1.f, 0);
            break;
        case Pacman::Direction::Up:
            move_dir = sf::Vector2f(0, -1.f);
            break;
        case Pacman::Direction::Down:
            move_dir = sf::Vector2f(0, 1.f);
            break;
    }

    // Move pacman to the wanted direction to check collision
    pacman_->move(move_dir);

    // Check if not exist collision with the map
    if (!this->checkMapCollision(pacman_->getCollisionBox())) {
        pacman_->setDirection(next_dir_);
        next_dir_ = Pacman::Direction::None;
    }

    // Return pacman to last position
    pacman_->move(move_dir * -1.f);
}

void PlayState::updateGhost(Ghost* ghost) {
    // Update pacman position and animation
    ghost->updatePos();
    ghost->updateAnimation();

    // Check if the ghost is not on a Tile, if not it checks if he want to go backwards
    if ((static_cast<int>(ghost->getCollisionBox().left) % 8) != 0 || 
        (static_cast<int>(ghost->getCollisionBox().top) % 8) != 0) {
        return;
    }

    // Positions to check
    std::vector<std::pair<Ghost::Direction, sf::Vector2f>> postocheck{
        {Ghost::Direction::Left, {-1.f, 0}},
        {Ghost::Direction::Right, {1.f, 0}},
        {Ghost::Direction::Up, {0, -1.f}},
        {Ghost::Direction::Down,  {0, 1.f}}
    };

    std::vector<Ghost::Direction> posible_paths;

    for (const auto &pos : postocheck) {
        if (pos.first != -ghost->getDirection()) {
            // Move the ghost to the wanted direction to check collision
            ghost->move(pos.second);

            // Check if no exist collision with the map
            if (!this->checkMapCollision(ghost->getCollisionBox())) {
                posible_paths.push_back(pos.first);
            }

            // Return the ghost to last position
            ghost->move(pos.second * -1.f);
        }
    }

    if (posible_paths.size() == 0) return;

    int rand_index = rand() % static_cast<unsigned int>(posible_paths.size());
    ghost->setDirection(posible_paths[rand_index]);
}

bool PlayState::checkFoodCollision() {
    sf::FloatRect pacman_box = pacman_->getCollisionBox();
    tmx::Layer& food_map = map_->GetLayer("FoodMap");

    // Iterate over all the FoodMap tiles
    for (unsigned int y = 0; y < food_map.GetHeight(); ++y) {
        for (unsigned int x = 0; x < food_map.GetWidth(); ++x) {

            tmx::Layer::Tile& tile = food_map.GetTile(x, y);

            // If the tile is not an empty tile
            if (!tile.empty()) {
                sf::FloatRect tile_box = tile.GetGlobalBounds();
                // If the tile is visible, it hide it
                if (tile.visible && pacman_box == tile_box) {
                    tile.visible = false;
                    return true;
                }
            }
        }
    }

    return false;
}

bool PlayState::AABBCollision(const sf::FloatRect& obj1, const sf::FloatRect& obj2) {
    if (obj1.left < (obj2.left + obj2.width) && obj1.top < (obj2.top + obj2.height) &&
        obj2.left < (obj1.left + obj1.width) && obj2.top < (obj1.top + obj1.height))
        return true;
    else
        return false;
    
}
