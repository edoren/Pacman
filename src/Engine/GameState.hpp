#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "Engine/GameEngine.hpp"
#include "Engine/ResourceManager.hpp"

class GameState {
 public:
    virtual void init(ResourceManager* resources, Settings* settings) = 0;
    virtual void exit(ResourceManager* resources) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void handleEvents(GameEngine* game) = 0;

    // Update state
    virtual void frameStarted(GameEngine* game) = 0;
    virtual void frameEnded(GameEngine* game) = 0;

    virtual void draw(GameEngine* game) = 0;

    virtual ~GameState();

 protected:
    GameState() {};

 private:
    GameState(const GameState&) = delete;
    GameState& operator =(const GameState&) = delete;
};

#endif  // GAME_STATE_HPP
