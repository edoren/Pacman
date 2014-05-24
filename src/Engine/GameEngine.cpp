#include "Engine/GameEngine.hpp"
#include "Engine/GameState.hpp"

GameEngine::GameEngine(sf::RenderWindow* window, const std::string& working_dir)
      : running_(true),
        working_dir_(working_dir),
        settings_(nullptr),
        window_(window),
        resources_(working_dir) {
}

GameEngine::~GameEngine() {
    delete settings_;
}

void GameEngine::init() {}

void GameEngine::cleanup() {
    // cleanup the all states
    while ( !states_.empty() ) {
        states_.top()->exit(&resources_);
        states_.pop();
    }
}

void GameEngine::changeState(GameState* state) {
    // exit the current state
    if (!states_.empty()) {
        states_.top()->exit(&resources_);
        states_.pop();
    }

    // store and init the new state
    states_.push(state);
    states_.top()->init(&resources_, settings_);
}

void GameEngine::pushState(GameState* state) {
    // pause the current state
    if ( !states_.empty() ) {
        states_.top()->pause();
    }

    // store and init the new state
    states_.push(state);
    states_.top()->init(&resources_, settings_);
}

void GameEngine::popState() {
    // exit the current state
    if (!states_.empty()) {
        states_.top()->exit(&resources_);
        states_.pop();
    }

    // resume the previous state
    if (!states_.empty()) {
        states_.top()->resume();
    }
}

void GameEngine::handleEvents() {
    // let the top state handle the events
    states_.top()->handleEvents(this);
}

void GameEngine::frameStarted() {
    states_.top()->frameStarted(this);
}

void GameEngine::frameEnded() {
    states_.top()->frameEnded(this);
}

void GameEngine::draw() {
    // let the top state draw the screen
    window_->clear();
    states_.top()->draw(this);
    window_->display();
}

bool GameEngine::isRunning() {
    return running_;
}

void GameEngine::quit() {
    running_ = false;
}

void GameEngine::startConfigFile(const std::string& config_file) {
    settings_ = new Settings(working_dir_ + config_file);
}

sf::RenderWindow* GameEngine::getWindow() {
    return window_;
}
