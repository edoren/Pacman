#include "PlayState.hpp"

PlayState PlayState::PlayState_;

PlayState::PlayState() {}

void PlayState::init(ResourceManager* resources) {
    // Pacman and Ghosts texture is always loaded while the game is open
    sf::Texture* pacman_texture = resources->loadTexture("assets/sprites/pacman/pacman.png");
    sf::Texture* ghosts_texture = resources->loadTexture("assets/sprites/ghosts/ghosts.png");

    const std::string& working_dir = resources->getWorkingDirectory();

    map_ = new tmx::TileMap(working_dir + "assets/maps/clasic.tmx");
    pacman_ = new Pacman(pacman_texture, working_dir);

    blinky_ = new Ghost(Ghost::Name::Blinky, ghosts_texture, working_dir);
    blinky_->setDirection(Ghost::Direction::Right);
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
                    pacman_->setDirection(Pacman::Direction::Up);
                    break;
                case sf::Keyboard::Down:
                    pacman_->setDirection(Pacman::Direction::Down);
                    break;
                case sf::Keyboard::Left:
                    pacman_->setDirection(Pacman::Direction::Left);
                    break;
                case sf::Keyboard::Right:
                    pacman_->setDirection(Pacman::Direction::Right);
                    break;
            }
        }
    }
}

void PlayState::frameStarted(GameEngine* game) {
    pacman_->updateAnimation();
    pacman_->updatePos();
    blinky_->updateAnimation();
    blinky_->updatePos();
}

void PlayState::frameEnded(GameEngine* game) {
}

void PlayState::draw(GameEngine* game) {
    sf::RenderWindow* window = game->getWindow();
    window->draw(*map_);
    window->draw(*blinky_);
    window->draw(*pacman_);
}
