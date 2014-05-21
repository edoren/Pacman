#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <stack>

#include "SFML/Graphics.hpp"

#include "Engine/ResourceManager.hpp"
#include "Engine/Settings.hpp"

class GameState;

class GameEngine {
 public:
    GameEngine(sf::RenderWindow* window, const std::string& working_dir = "./");
    ~GameEngine();

    void init();
    void cleanup();

    void changeState(GameState* state);
    void pushState(GameState* state);
    void popState();

    void handleEvents();

    void frameStarted();
    void frameEnded();

    void draw();

    bool isRunning();
    void quit();

    void startConfigFile(const std::string& config_file = "config.json");
    sf::RenderWindow* getWindow();

 private:
    std::string working_dir_;

    Settings* settings_;
    ResourceManager resources_;
    std::stack<GameState*> states_;

    bool running_;

    sf::RenderWindow* window_;
};

#endif  // GAME_ENGINE_HPP
