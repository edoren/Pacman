#ifndef INTRO_STATE_HPP
#define INTRO_STATE_HPP

#include "Engine/GameState.hpp"
#include "Engine/Utils/Clock.hpp"

class IntroState : public GameState {
 public:
    void init(ResourceManager* resources);
    void exit(ResourceManager* resources);
    void pause();
    void resume();

    void handleEvents(GameEngine* game);

    // Update state
    void frameStarted(GameEngine* game);
    void frameEnded(GameEngine* game);

    void draw(GameEngine* game);

    static IntroState* Instance() {
        return &IntroState_;
    }

 protected:
    IntroState();

 private:
    static IntroState IntroState_;

    sf::RectangleShape* background_;
    sf::Sprite* sfml_logo_;

    Clock* intro_clock_;
};

#endif  // INTRO_STATE_HPP
