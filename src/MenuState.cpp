#include "MenuState.hpp"

#include "PlayState.hpp"

MenuState MenuState::MenuState_;

MenuState::MenuState() : 
        selected_(0),
        background_(nullptr),
        pacman_logo_(nullptr),
        options_(nullptr),
        pacman_(nullptr),
        blinky_(nullptr),
        inky_(nullptr),
        clyde_(nullptr),
        pinky_(nullptr) {}

void MenuState::init(ResourceManager* resources) {
    ///////////////////////////////
    // Initialize the menu items //
    ///////////////////////////////

    sf::Texture *bg_texture = resources->loadTexture("assets/images/menu.png");
    background_ = new sf::Sprite(*bg_texture);

    sf::Texture *pacman_logo_texture = resources->loadTexture("assets/images/pacman-logo.png");
    pacman_logo_ = new sf::Sprite(*pacman_logo_texture);
    pacman_logo_->setOrigin({pacman_logo_texture->getSize().x / 2.f, 0.f});
    pacman_logo_->setPosition({224 / 2.f}, 50);

    options_ = new std::vector<std::pair<GameState*, sf::Text>>();
    options_->reserve(3);

    sf::Font* menuFont = resources->loadFont("assets/fonts/prstartk.ttf");

    sf::Vector2f menuPos(224 / 2.f, 130.f);
    sf::Vector2f spacing(0, 13.f);  // Spacing between the menu elements

    sf::Text startText("Play", *menuFont, 15);
    sf::Text scoresText("Scores", *menuFont, 15);
    sf::Text optionsText("Options", *menuFont, 15);

    startText.setColor(sf::Color::Blue);  //  Set blue the selected option

    options_->push_back({PlayState::Instance(), startText});
    options_->push_back({PlayState::Instance(), scoresText});
    options_->push_back({PlayState::Instance(), optionsText});

    for (unsigned int i = 0; i < options_->size(); i++) {
        sf::Text* text = &(options_->at(i).second);
        text->setOrigin({text->getLocalBounds().width / 2.f, 0.f});
        text->setPosition(menuPos + sf::Vector2f(0, i * text->getLocalBounds().height) + static_cast<float>(i) * spacing);
    }

    ///////////////////////////////
    // Initialize the animation  //
    ///////////////////////////////

    sf::Vector2f start_pos(224 + 14 + 5, 245);  // x = window size + sprite size + offset
    sf::Vector2f distance(14 + 6, 0);  // x = sprite size + distance

    // Pacman and Ghosts texture is always loaded while the game is open
    sf::Texture* pacman_texture = resources->loadTexture("assets/sprites/pacman/pacman.png");
    sf::Texture* ghosts_texture = resources->loadTexture("assets/sprites/ghosts/ghosts.png");

    pacman_ = new Pacman(pacman_texture);

    pacman_->setPosition(start_pos);
    pacman_->setDirection(Pacman::Direction::Left);

    blinky_ = new Ghost(Ghost::Name::Blinky, ghosts_texture);
    pinky_ = new Ghost(Ghost::Name::Pinky, ghosts_texture);
    inky_ = new Ghost(Ghost::Name::Inky, ghosts_texture);
    clyde_ = new Ghost(Ghost::Name::Clyde, ghosts_texture);

    blinky_->setDirection(Ghost::Direction::Left);
    pinky_->setDirection(Ghost::Direction::Left);
    inky_->setDirection(Ghost::Direction::Left);
    clyde_->setDirection(Ghost::Direction::Left);

    blinky_->setPosition(start_pos + distance * 1.f);
    pinky_->setPosition(start_pos + distance * 2.f);
    inky_->setPosition(start_pos + distance * 3.f);
    clyde_->setPosition(start_pos + distance * 4.f);
};

void MenuState::exit(ResourceManager* resources) {
    delete background_;
    resources->freeTexture("assets/images/menu.png");
    delete pacman_logo_;
    resources->freeTexture("assets/images/pacman-logo.png");
    delete options_;
    resources->freeFont("assets/fonts/prstartk.ttf");

    delete pacman_;
    delete blinky_;
    delete inky_;
    delete clyde_;
    delete pinky_;
};

void MenuState::pause() {
}; 

void MenuState::resume() {
};

void MenuState::handleEvents(GameEngine* game) {
    sf::RenderWindow* window = game->getWindow();
    sf::Event event;
    while (window->pollEvent(event)) {
        // Close window : exit
        if (event.type == sf::Event::Closed ||
            (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) ||
            (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::F4)))
            game->quit();
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Up:  // Change the selection up
                    options_->at(selected_).second.setColor(sf::Color::White);
                    selected_ -= 1; 
                    if (selected_ < 0) selected_ = options_->size() - 1;
                    options_->at(selected_).second.setColor(sf::Color::Blue);
                    break;
                case sf::Keyboard::Down:  // Change the selection down
                    options_->at(selected_).second.setColor(sf::Color::White);
                    selected_ += 1;
                    if (selected_ >= options_->size()) selected_ = 0;
                    options_->at(selected_).second.setColor(sf::Color::Blue);
                    break; 
                case sf::Keyboard::Return:
                    game->pushState(options_->at(selected_).first);
            }
        }
    }
}

void MenuState::frameStarted(GameEngine* game) {
    pacman_->update_animation();
    pacman_->update_pos();

    blinky_->update_animation();
    pinky_->update_animation();
    inky_->update_animation();
    clyde_->update_animation();

    blinky_->update_pos();
    pinky_->update_pos();
    inky_->update_pos();
    clyde_->update_pos();
}

void MenuState::frameEnded(GameEngine* game) {
    if (pacman_->getPosition().x <= -(14 * 4.f + 6 * 3.f + 200.f)) {
        sf::Vector2f start_pos(-(14 * 4.f + 6 * 3.f + 20), 245);  // x = -(sprite size * 4 + sprite distance * 3 + offset)
        sf::Vector2f distance(14 + 6, 0);  // x = sprite size + distance

        pacman_->setPosition(start_pos);
        pacman_->setDirection(Pacman::Direction::Right);

        blinky_->setDirection(Ghost::Direction::Right);
        pinky_->setDirection(Ghost::Direction::Right);
        inky_->setDirection(Ghost::Direction::Right);
        clyde_->setDirection(Ghost::Direction::Right);

        blinky_->setState(Ghost::State::Frightened);
        pinky_->setState(Ghost::State::Frightened);
        inky_->setState(Ghost::State::Frightened);
        clyde_->setState(Ghost::State::Frightened);

        blinky_->setPosition(start_pos + distance * 1.f);
        pinky_->setPosition(start_pos + distance * 2.f);
        inky_->setPosition(start_pos + distance * 3.f);
        clyde_->setPosition(start_pos + distance * 4.f);
    } else if (pacman_->getPosition().x >= (224 + 14 * 4.f + 6 * 3.f + 500.f)) {
        sf::Vector2f start_pos(224 + 14 + 5, 245);  // x = window size + sprite size + offset
        sf::Vector2f distance(14 + 6, 0);  // x = sprite size + distance

        pacman_->setPosition(start_pos);
        pacman_->setDirection(Pacman::Direction::Left);

        blinky_->setDirection(Ghost::Direction::Left);
        pinky_->setDirection(Ghost::Direction::Left);
        inky_->setDirection(Ghost::Direction::Left);
        clyde_->setDirection(Ghost::Direction::Left);

        blinky_->setState(Ghost::State::Normal);
        pinky_->setState(Ghost::State::Normal);
        inky_->setState(Ghost::State::Normal);
        clyde_->setState(Ghost::State::Normal);

        blinky_->setPosition(start_pos + distance * 1.f);
        pinky_->setPosition(start_pos + distance * 2.f);
        inky_->setPosition(start_pos + distance * 3.f);
        clyde_->setPosition(start_pos + distance * 4.f);
    }
}

void MenuState::draw(GameEngine* game) {
    sf::RenderWindow* window = game->getWindow();
    window->draw(*background_);
    window->draw(*pacman_logo_);
    for(auto option : *options_) {
        window->draw(option.second);
    }

    window->draw(*pacman_);

    window->draw(*blinky_);
    window->draw(*pinky_);
    window->draw(*inky_);
    window->draw(*clyde_);
}
