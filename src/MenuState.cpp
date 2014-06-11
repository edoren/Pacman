#include "MenuState.hpp"

#include "PlayState.hpp"

MenuState MenuState::MenuState_;

MenuState::MenuState() :
        background_(nullptr),
        pacman_logo_(nullptr),
        pacman_(nullptr),
        blinky_(nullptr),
        inky_(nullptr),
        clyde_(nullptr),
        pinky_(nullptr),
        selected_(0),
        options_(nullptr) {
}

void MenuState::init(ResourceManager* resources, Settings* settings) {
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

    // Pacman and Ghosts texture are always loaded while the game is open
    sf::Texture* pacman_texture = resources->loadTexture("assets/sprites/pacman/pacman.png");
    sf::Texture* ghosts_texture = resources->loadTexture("assets/sprites/ghosts/ghosts.png");

    const std::string& working_dir = resources->getWorkingDirectory();

    pacman_ = new Pacman(pacman_texture, resources->getWorkingDirectory());

    blinky_ = new Ghost(Ghost::Name::Blinky, ghosts_texture, {}, working_dir);
    pinky_ = new Ghost(Ghost::Name::Pinky, ghosts_texture, {}, working_dir);
    inky_ = new Ghost(Ghost::Name::Inky, ghosts_texture, {}, working_dir);
    clyde_ = new Ghost(Ghost::Name::Clyde, ghosts_texture, {}, working_dir);

    sf::Vector2f start_pos(224 + 14 + 100, 245);  // x = window size + sprite size + offset
    this->setAnimationRight(start_pos);
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
    // Restart and reset the animation position
    pacman_->restartAnimation();

    blinky_->restartAnimation();
    pinky_->restartAnimation();
    inky_->restartAnimation();
    clyde_->restartAnimation();

    sf::Vector2f start_pos(224 + 14 + 100, 245);  // x = window size + sprite size + offset
    this->setAnimationRight(start_pos);
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
                    if (selected_ >= static_cast<int>(options_->size())) selected_ = 0;
                    options_->at(selected_).second.setColor(sf::Color::Blue);
                    break;
                case sf::Keyboard::Return:
                    game->pushState(options_->at(selected_).first);
                default:
                    break;
            }
        }
    }
}

void MenuState::frameStarted(GameEngine* game) {
    pacman_->updateAnimation();
    pacman_->updatePos();

    blinky_->updateAnimation();
    pinky_->updateAnimation();
    inky_->updateAnimation();
    clyde_->updateAnimation();

    blinky_->updatePos();
    pinky_->updatePos();
    inky_->updatePos();
    clyde_->updatePos();
}

void MenuState::frameEnded(GameEngine* game) {
    if (pacman_->getPosition().x <= -(14 * 4.f + 6 * 3.f + 200.f)) {
        sf::Vector2f pos(-(14 * 4.f + 6 * 3.f + 20), 245);  // x = -(sprite size * 4 + sprite distance * 3 + offset)
        this->setAnimationLeft(pos);
    } else if (pacman_->getPosition().x >= (224 + 14 * 4.f + 6 * 3.f + 300.f)) {
        sf::Vector2f pos(224 + 14 + 5, 245);  // x = window size + sprite size + offset
        this->setAnimationRight(pos);
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

void MenuState::setAnimationLeft(sf::Vector2f& left_pos) {
    sf::Vector2f distance(14 + 6, 0);  // x = sprite size + distance

    pacman_->setPosition(left_pos);
    pacman_->setDirection(Pacman::Direction::Right);

    blinky_->setDirection(Ghost::Direction::Right);
    pinky_->setDirection(Ghost::Direction::Right);
    inky_->setDirection(Ghost::Direction::Right);
    clyde_->setDirection(Ghost::Direction::Right);

    blinky_->setState(Ghost::State::Frightened);
    pinky_->setState(Ghost::State::Frightened);
    inky_->setState(Ghost::State::Frightened);
    clyde_->setState(Ghost::State::Frightened);

    blinky_->setPosition(left_pos + distance * 1.f);
    pinky_->setPosition(left_pos + distance * 2.f);
    inky_->setPosition(left_pos + distance * 3.f);
    clyde_->setPosition(left_pos + distance * 4.f);
}

void MenuState::setAnimationRight(sf::Vector2f& right_pos) {
    sf::Vector2f distance(14 + 6, 0);  // x = sprite size + distance

    pacman_->setPosition(right_pos);
    pacman_->setDirection(Pacman::Direction::Left);

    blinky_->setDirection(Ghost::Direction::Left);
    pinky_->setDirection(Ghost::Direction::Left);
    inky_->setDirection(Ghost::Direction::Left);
    clyde_->setDirection(Ghost::Direction::Left);

    blinky_->setState(Ghost::State::Chase);
    pinky_->setState(Ghost::State::Chase);
    inky_->setState(Ghost::State::Chase);
    clyde_->setState(Ghost::State::Chase);

    blinky_->setPosition(right_pos + distance * 1.f);
    pinky_->setPosition(right_pos + distance * 2.f);
    inky_->setPosition(right_pos + distance * 3.f);
    clyde_->setPosition(right_pos + distance * 4.f);
}
