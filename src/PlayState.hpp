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

	int food_amount_;
	float map_opacity_;

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
	Clock* win_clock_;
    Clock* die_clock_;
	Clock* map_blink_clock_;

    bool pacman_die_;

    Pacman::Direction next_dir_;

 private:
	void win();
	void lose();

	void restart(bool restart_score = false);

    // Update pacman and verify collisions
    void updatePacman();
    void updateGhost(Ghost* ghost);
};

#endif  // PLAY_STATE_HPP
