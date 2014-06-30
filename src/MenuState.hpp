#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include <vector> // std::vector
#include <utility> // std::pair

#include "Engine/GameState.hpp"

#include "Pacman.hpp"
#include "Ghost.hpp"

class MenuState : public GameState {
 public:
    void init(ResourceManager* resources, Settings* settings);
    void exit(ResourceManager* resources);
    void pause();
    void resume();

    void handleEvents(GameEngine* game);

    // Update state
    void frameStarted(GameEngine* game);
    void frameEnded(GameEngine* game);

    void draw(GameEngine* game);

    static MenuState* Instance() {
        return &MenuState_;
    }

 protected:
    MenuState();

 private:
    static MenuState MenuState_;

    sf::Sprite* background_;
    sf::Sprite* pacman_logo_;

    Pacman* pacman_;
    Ghost* blinky_;
    Ghost* inky_;
    Ghost* clyde_;
    Ghost* pinky_;

    sf::Vector2f velocity_;

    int selected_;
    std::vector<std::pair<GameState*, sf::Text>>* options_;

    void updateAnimation();
    void setAnimationLeft(sf::Vector2f& left_pos);
    void setAnimationRight(sf::Vector2f& right_pos);
};

#endif  // MENU_STATE_HPP
