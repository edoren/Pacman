#include "IntroState.hpp"

#include "MenuState.hpp"

IntroState IntroState::IntroState_;

IntroState::IntroState() {}

void IntroState::init(ResourceManager* resources) {
    sf::Texture* sfml_texture = resources->loadTexture("assets/images/sfml-logo.png");

    background_ = new sf::RectangleShape(sf::Vector2f(224, 288));
    background_->setFillColor(sf::Color::Black);

    sfml_logo_ = new sf::Sprite(*sfml_texture);
    sfml_logo_->setOrigin(sf::Vector2f(sfml_texture->getSize().x, sfml_texture->getSize().y) / 2.f);
    sfml_logo_->setPosition(background_->getSize() / 2.f);

    intro_clock_ = new Clock();
};

void IntroState::exit(ResourceManager* resources) {
    delete background_;
    delete sfml_logo_;
    resources->freeTexture("assets/images/sfml-logo.png");

    delete intro_clock_;
};

void IntroState::pause() {
    intro_clock_->pause();
}; 

void IntroState::resume() {
    intro_clock_->resume();
};

void IntroState::handleEvents(GameEngine* game) {
    sf::RenderWindow* window = game->getWindow();
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
            (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) ||
            (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::F4)))
            game->quit();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                game->changeState(MenuState::Instance());
                break;
            }
        }
    }
}

void IntroState::frameStarted(GameEngine* game) {}

void IntroState::frameEnded(GameEngine* game) {
    if (intro_clock_->getElapsedTime().asSeconds() > 5.f)
        game->changeState(MenuState::Instance());
}

void IntroState::draw(GameEngine* game) {
    sf::RenderWindow* window = game->getWindow();
    window->draw(*background_);
    window->draw(*sfml_logo_);
}
