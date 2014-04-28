#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include "STP/TMXLoader.hpp"

#include "Engine/GameState.hpp"
#include "Engine/Utils/Clock.hpp"

#include "Pacman.hpp"
#include "Ghost.hpp"

class PlayState : public GameState {
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

    static PlayState* Instance() {
        return &PlayState_;
    }

 protected:
    PlayState();

 private:
    static PlayState PlayState_;

    Pacman* pacman_;
    Ghost* blinky_;
    tmx::TileMap* map_;

 private:
    bool checkMapCollision();
};

#endif  // PLAY_STATE_HPP
