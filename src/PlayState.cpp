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
    sf::SoundBuffer* intermission_sound_buffer = resources->loadSoundBuffer("assets/sounds/intermission.wav");

    const std::string& working_dir = resources->getWorkingDirectory();

    // Load the map
    map_ = new tmx::TileMap(working_dir + "assets/maps/clasic.tmx");
	map_opacity_ = 1.f;

    // Na Na Na Na Na Na Pacman!
    pacman_ = new Pacman(pacman_texture, working_dir, {105, 208 - 3});

    sf::FloatRect house_bounds = {80, 120, 64, 40};

    // The Ghosts!
    blinky_ = new Ghost(Ghost::Name::Blinky, ghosts_texture, house_bounds, working_dir);
    inky_ = new Ghost(Ghost::Name::Inky, ghosts_texture, house_bounds, working_dir);
    pinky_ = new Ghost(Ghost::Name::Pinky, ghosts_texture, house_bounds, working_dir);
    clyde_ = new Ghost(Ghost::Name::Clyde, ghosts_texture, house_bounds, working_dir);

    // Add the sounds
    start_sound_ = new sf::Sound(*start_sound_buffer);
    lose_sound_ = new sf::Sound(*lose_sound_buffer);
    siren_sound_ = new sf::Sound(*siren_sound_buffer);
    chomp_sound_[0] = new sf::Sound(*chomp1_sound_buffer);
    chomp_sound_[1] = new sf::Sound(*chomp2_sound_buffer);
    intermission_sound_ = new sf::Sound(*intermission_sound_buffer);

    siren_sound_->setLoop(true);

    start_clock_ = new Clock();
	win_clock_ = new Clock(false); // Stopped clock
    die_clock_ = new Clock(false); // Stopped clock
	map_blink_clock_ = new Clock(false); // Stopped clock

    // Set the settings
    float volume = settings->getSetting("volume", 100.f);

    start_sound_->setVolume(50.f * volume / 100.f);
    lose_sound_->setVolume(50.f * volume / 100.f);
    siren_sound_->setVolume(40.f * volume/100.f);
    chomp_sound_[0]->setVolume(50.f * volume/100.f);
    chomp_sound_[1]->setVolume(50.f * volume/100.f);
    intermission_sound_->setVolume(40.f * volume/100.f);

    start_sound_->play();

    pacman_die_ = false;

	// Calculate the food amount
	food_amount_ = 0;
	tmx::Layer& food_map = map_->GetLayer("FoodMap");

	for (unsigned int y = 0; y < food_map.GetHeight(); ++y) {
		for (unsigned int x = 0; x < food_map.GetWidth(); ++x) {

			tmx::Layer::Tile& tile = food_map.GetTile(x, y);

			// If the tile is not an empty tile
			if (!tile.empty())
				food_amount_++;
		}
	}
};

void PlayState::exit(ResourceManager* resources) {
    delete pacman_;

    delete blinky_;
    delete inky_;
    delete pinky_;
    delete clyde_;

    delete map_;

    // Free the sounds
    resources->freeSoundBuffer("assets/sounds/start-game.wav");
    resources->freeSoundBuffer("assets/sounds/pacman-lose.wav");
    resources->freeSoundBuffer("assets/sounds/siren.wav");
    resources->freeSoundBuffer("assets/sounds/chomp1.wav");
    resources->freeSoundBuffer("assets/sounds/chomp2.wav");
    resources->freeSoundBuffer("assets/sounds/intermission.wav");

    delete start_clock_;
    delete die_clock_;
	delete win_clock_;
	delete map_blink_clock_;
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
		if (pacman_->isAlive() && win_clock_->getElapsedTime().asSeconds() <= 0.f && event.type == sf::Event::KeyPressed) {
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
        // Start the state timers of the ghosts
        for (auto& ghost : Ghost::ghosts) ghost.second->state_timer.resume();

        // Replay the siren sound after the intermission sound finish
        if (pacman_->isAlive()) {
            if (intermission_sound_->getPlayingOffset().asSeconds() > 0) {
                siren_sound_->stop();
            } else if (siren_sound_->getPlayingOffset().asSeconds() == 0) {
                siren_sound_->play();
            }
        }

        if (!pacman_die_ && die_clock_->getElapsedTime().asSeconds() > 0.5) {
            lose_sound_->play();  // Play the lose sound
            pacman_->resume();  // Resume the animation
            pacman_die_ = true;
        }

        if (die_clock_->getElapsedTime().asSeconds() > 2.5) {
            restart();
        }

        this->updatePacman();
        this->updateGhost(blinky_);
        this->updateGhost(inky_);
        this->updateGhost(pinky_);
        this->updateGhost(clyde_);
    }
}

void PlayState::frameEnded(GameEngine* game) {
	tmx::Layer& food_map = map_->GetLayer("FoodMap");

	if (win_clock_->getElapsedTime().asSeconds() > 2.f) {
		///////////////////////////////////////////////
		// Should change to another state
		///////////////////////////////////////////////

		food_amount_ = 0;
		map_->GetLayer("PacmanMap").SetOpacity(1.f);

		for (unsigned int y = 0; y < food_map.GetHeight(); ++y) {
			for (unsigned int x = 0; x < food_map.GetWidth(); ++x) {

				tmx::Layer::Tile& tile = food_map.GetTile(x, y);

				// If the tile is not an empty tile
				if (!tile.empty()) {
					food_amount_++;
					tile.visible = true;
				}
			}
		}

		this->restart(true);
	}
}

void PlayState::draw(GameEngine* game) {
    sf::RenderWindow* window = game->getWindow();
    window->draw(*map_);
    window->draw(*pacman_);

	if (win_clock_->getElapsedTime().asSeconds() > 0) {
		tmx::Layer& pacman_map = map_->GetLayer("PacmanMap");
		if (map_blink_clock_->getElapsedTime().asSeconds() > 0.2) {
			pacman_map.SetOpacity(map_opacity_);

			if (map_opacity_ < 1.f) map_opacity_ = 1.f;
			else map_opacity_ = 0.1f;
			
			map_blink_clock_->restart();
		}
		return;
	}

    if (start_clock_->getElapsedTime().asSeconds() >= 2.0f && die_clock_->getElapsedTime().asSeconds() < 0.5) {
        for (auto& ghost : Ghost::ghosts) {
            sf::Vector2f pos = ghost.second->getPosition();
            ghost.second->setPosition(floorf(pos.x), floorf(pos.y));
            window->draw(*ghost.second);
            ghost.second->setPosition(pos);
        }
    }
}

void PlayState::win() {
	// Stop the sounds
	siren_sound_->stop();
	chomp_sound_[0]->stop();
	chomp_sound_[1]->stop();
	intermission_sound_->stop();

	for (auto& ghost : Ghost::ghosts) ghost.second->pause();  // Pause the ghosts
	pacman_->pause();  // Pause pacman

	win_clock_->resume();
	map_blink_clock_->resume();
};

void PlayState::lose() {
	for (auto& ghost : Ghost::ghosts) ghost.second->pause();  // Pause the ghosts

	// Stop the sounds
	siren_sound_->stop();
	chomp_sound_[0]->stop();
	chomp_sound_[1]->stop();
	intermission_sound_->stop();

	pacman_->kill();  // Kills Pacman
	pacman_->pause();  // Pause the animation
	die_clock_->resume();
};

void PlayState::restart(bool restart_score) {
    start_clock_->restart();
	win_clock_->stop();
	die_clock_->stop();
	map_blink_clock_->stop();

	// if (restart_score) score_ = 0;

    pacman_die_ = false;

    pacman_->restart();
	pacman_->resume();
    for (auto& ghost : Ghost::ghosts) ghost.second->restart();
};

void PlayState::updatePacman() {
    // Update pacman position and animation
    pacman_->updatePos();
    pacman_->updateAnimation();

    // Check pacman collision with each ghost
    for (auto& ghost : Ghost::ghosts) {
        if (Collision::AABBCollision(pacman_->getCollisionBox(), ghost.second->getCollisionBox())) {
            if (ghost.second->getState() == Ghost::Frightened) {
                ghost.second->setState(Ghost::State::ToHouse);
            } else if (ghost.second->getState() == Ghost::Chase || ghost.second->getState() == Ghost::Scatter || ghost.second->getState() == Ghost::InHouse) {
                if (pacman_->isAlive()) {
					this->lose();
                }
            }
        };
    }

    /////////////////////////////////////////////////////////////////////////////////
    // Check if pacman is not on a Tile, if not it checks if he want to go backwards
    /////////////////////////////////////////////////////////////////////////////////
    if ((static_cast<int>(pacman_->getCollisionBox().left) % 8) != 0 ||
        (static_cast<int>(pacman_->getCollisionBox().top) % 8) != 0) {
        if (pacman_->getDirection() == -next_dir_) {
            pacman_->setDirection(next_dir_);
            next_dir_ = Pacman::Direction::None;
        }
        return;
    }


    //* Check pacman collision with the food *//
    static bool chomp = 0;
    int food = Collision::checkFoodCollision(map_, pacman_);
    if (food) {
        if (food_amount_ == 0) this->win();

        if (food == 2) {
            intermission_sound_->play();
            for (auto& ghost : Ghost::ghosts) ghost.second->setState(Ghost::State::Frightened);
        }

        // Play chomp sound
        chomp_sound_[chomp]->play();
        chomp = !chomp;
    }


    //* Check pacman collision with the map *//

    // Font tile
    sf::Vector2f front_tile_pos = {
        (pacman_->getCollisionBox().left / 8) + pacman_->getVelocity().x, 
        (pacman_->getCollisionBox().top / 8) + pacman_->getVelocity().y 
    };
    if (Collision::checkMapCollision(map_, front_tile_pos) && pacman_->isAlive()) {
        pacman_->pause();
    }

    // Direction tile
    sf::Vector2f next_tile_pos = {
        (pacman_->getCollisionBox().left / 8),
        (pacman_->getCollisionBox().top / 8)
    };
    switch(next_dir_) {
        case Pacman::Direction::None:
            return;
        case Pacman::Direction::Left:
            next_tile_pos += sf::Vector2f{ -1.f, 0 };
            break;
        case Pacman::Direction::Right:
            next_tile_pos += sf::Vector2f{ 1.f, 0 };
            break;
        case Pacman::Direction::Up:
            next_tile_pos += sf::Vector2f{ 0, -1.f };
            break;
        case Pacman::Direction::Down:
            next_tile_pos += sf::Vector2f{ 0, 1.f };
            break;
    }
    if (!Collision::checkMapCollision(map_, next_tile_pos)) {
        pacman_->setDirection(next_dir_);
        next_dir_ = Pacman::Direction::None;
        // Resume pacman if is paused
        if (pacman_->is_paused()) pacman_->resume();
    }
}

void PlayState::updateGhost(Ghost* ghost) {
    // Update pacman position and animation
    ghost->updatePos(map_, pacman_->getPosition(), pacman_->getDirection());
    ghost->updateAnimation();
}
