#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include "SFML/Audio/Sound.hpp"
#include "STP/TMXLoader.hpp"

#include "Engine/GameState.hpp"
#include "Engine/Utils/Clock.hpp"

#include "Pacman.hpp"
#include "Ghost.hpp"

class PlayState : public GameState {
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

    static PlayState* Instance() {
        return &PlayState_;
    }

 protected:
    PlayState() {};

 private:
    static PlayState PlayState_;

    Pacman* pacman_;

    Ghost* blinky_;
    Ghost* inky_;
    Ghost* pinky_;
    Ghost* clyde_;

    tmx::TileMap* map_;

    // Sounds
    sf::Sound* start_sound_;
    sf::Sound* lose_sound_;
    sf::Sound* siren_sound_;
    sf::Sound* chomp_sound_[2];
    sf::Sound* intermission_sound_;

    // Timmers
    Clock* start_clock_;

    bool is_playing_;

    Pacman::Direction next_dir_;

 private:
    // Update pacman and verify collisions
    void updatePacman();
    void updateGhost(Ghost* ghost);
};

#endif  // PLAY_STATE_HPP
