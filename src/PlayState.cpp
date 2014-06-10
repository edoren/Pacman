#include <cstdlib>

#include <vector>
#include <utility>

#include "PlayState.hpp"
#include "Collision.hpp"

PlayState PlayState::PlayState_;

void PlayState::init(ResourceManager* resources, Settings* settings) {
    // Pacman and Ghosts texture is always loaded while the game is open
    sf::Texture* pacman_texture = resources->loadTexture("assets/sprites/pacman/pacman.png");
    sf::Texture* ghosts_texture = resources->loadTexture("assets/sprites/ghosts/ghosts.png");

    // Load the sounds in memory
    sf::SoundBuffer* start_sound_buffer = resources->loadSoundBuffer("assets/sounds/start-game.wav");
    sf::SoundBuffer* lose_sound_buffer = resources->loadSoundBuffer("assets/sounds/pacman-lose.wav");
    sf::SoundBuffer* siren_sound_buffer = resources->loadSoundBuffer("assets/sounds/siren.wav");
    sf::SoundBuffer* chomp1_sound_buffer = resources->loadSoundBuffer("assets/sounds/chomp1.wav");
    sf::SoundBuffer* chomp2_sound_buffer = resources->loadSoundBuffer("assets/sounds/chomp2.wav");

    const std::string& working_dir = resources->getWorkingDirectory();

    map_ = new tmx::TileMap(working_dir + "assets/maps/clasic.tmx");
    pacman_ = new Pacman(pacman_texture, working_dir);

    pacman_->setPosition({112 - 3, 208 - 3});

    blinky_ = new Ghost(Ghost::Name::Blinky, ghosts_texture, working_dir);
    blinky_->setDirection(Ghost::Direction::Right);

    blinky_->setPosition({112 - 3, 112 - 3});

    // Add the sounds
    start_sound_ = new sf::Sound(*start_sound_buffer);
    lose_sound_ = new sf::Sound(*lose_sound_buffer);
    siren_sound_ = new sf::Sound(*siren_sound_buffer);
    chomp_sound_[0] = new sf::Sound(*chomp1_sound_buffer);
    chomp_sound_[1] = new sf::Sound(*chomp2_sound_buffer);

    siren_sound_->setLoop(true);

    start_clock_ = new Clock();
    is_playing_ = false;

    // Set the settings
    float volume = settings->getSetting("volume", 100.f);

    start_sound_->setVolume(volume);
    siren_sound_->setVolume(volume);
    chomp_sound_[0]->setVolume(volume);
    chomp_sound_[1]->setVolume(volume);

    start_sound_->play();
};

void PlayState::exit(ResourceManager* resources) {
    delete pacman_;
    delete blinky_;
    delete map_;

    // Free the sounds
    resources->freeSoundBuffer("assets/sounds/start-game.wav");
    resources->freeSoundBuffer("assets/sounds/pacman-lose.wav");
    resources->freeSoundBuffer("assets/sounds/siren.wav");
    resources->freeSoundBuffer("assets/sounds/chomp1.wav");
    resources->freeSoundBuffer("assets/sounds/chomp2.wav");

    delete start_clock_;
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
                default:
                    break;
            }
        }
    }
}

void PlayState::frameStarted(GameEngine* game) {
    if (start_clock_->getElapsedTime().asSeconds() >= 4.5f) {
        static bool siren_playing = false;
        if (!siren_playing) {
            siren_sound_->play();
            siren_playing = true;
        }
        this->updatePacman();
        this->updateGhost(blinky_);
    }
}

void PlayState::frameEnded(GameEngine* game) {
}

void PlayState::draw(GameEngine* game) {
    sf::RenderWindow* window = game->getWindow();
    window->draw(*map_);
    window->draw(*pacman_);
    if (start_clock_->getElapsedTime().asSeconds() >= 2.0f) {
        window->draw(*blinky_);
    }
}

void PlayState::updatePacman() {
    // Update pacman position and animation
    pacman_->updatePos();
    pacman_->updateAnimation();

    // Check pacman collision with the food
    static bool chomp = 0;
    if (Collision::checkFoodCollision(map_, pacman_)) {
        // Play chomp sound
        chomp_sound_[chomp]->play();
        chomp = !chomp;
    };

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

    // Move pacman to the front
    pacman_->move(pacman_->getVelocity());
    // Check if exist collision with the map
    if (Collision::checkMapCollision(map_, pacman_->getCollisionBox())) {
        pacman_->pause(); // Pause pacman movement and animation
    }
    // Return pacman to last position
    pacman_->move(pacman_->getVelocity() * -1.f);


    sf::Vector2f move_dir;

    // Select the move offset according to the pacman's direction
    switch(next_dir_) {
        case Pacman::Direction::None:
            return;
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
    if (!Collision::checkMapCollision(map_, pacman_->getCollisionBox())) {
        pacman_->setDirection(next_dir_);
        next_dir_ = Pacman::Direction::None;
        // Resume pacman if is paused
        if (pacman_->is_paused()) pacman_->resume();
    }

    // Return pacman to last position
    pacman_->move(move_dir * -1.f);
}

void PlayState::updateGhost(Ghost* ghost) {
    // Update pacman position and animation
    ghost->updatePos();
    ghost->updateAnimation();

    // Check if the ghost is not on a Tile
    if ((static_cast<int>(ghost->getCollisionBox().left) % 8) != 0 ||
        (static_cast<int>(ghost->getCollisionBox().top) % 8) != 0) {
        return;
    }

    ghost->focusMovement(map_, pacman_->getPosition());
}
