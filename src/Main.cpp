#include "SFML/Graphics.hpp"
#include "STP/TMXLoader.hpp"

#include "Engine/GameEngine.hpp"
#include "Pacman.hpp"
#include "IntroState.hpp"

#ifdef _WIN32
    #include <windows.h>
#endif

int main(int argc, char* argv[])
{
    std::string working_dir;

    #ifdef _WIN32
        char buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, sizeof(buffer));
        working_dir = buffer;
        working_dir = working_dir.substr(0, working_dir.find_last_of('\\') + 1);
    #else
        working_dir = argv[0];
        working_dir = working_dir.substr(0, working_dir.find_last_of('/') + 1);
    #endif

    sf::RenderWindow window(sf::VideoMode(224, 288), "Pacman", sf::Style::Close);
    window.setFramerateLimit(60);

    GameEngine game(&window, working_dir);
    game.startConfigFile("config.json");

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
