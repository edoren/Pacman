#include "SFML/Graphics.hpp"
#include "STP/TMXLoader.hpp"

#include "Engine/GameEngine.hpp"
#include "Pacman.hpp"
#include "IntroState.hpp"

int main(int argc, char* argv[])
{
    // pacmanPath = argv[0];
    // pacmanPath = pacmanPath.substr(0, pacmanPath.find_last_of(PATH_SEPARATOR)+1);

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(224, 288), "Pacman");
    window.setFramerateLimit(60);

    GameEngine game(&window);

    // load the game
    game.changeState(IntroState::Instance());

    // main loop
    while (game.isRunning()) {
        game.handleEvents();
        game.frameStarted();
        game.draw();
        game.frameEnded();
    }

    // cleanup the engine
    game.cleanup();
    window.close();

    return EXIT_SUCCESS;
}
