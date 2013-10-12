// Pacman, Ghost and FoodMap module
#include "Pacman.hpp"
#include "Ghost.hpp"
#include "TileMap.hpp"
// Sound Module
#include "Sound.hpp"
// Configuration module
#include "Config.hpp"

std::string pacmanPath;
bool firstStart = true;
sf::Clock timeOpen;

bool CollisionWithGhosts(Pacman *pacman, std::vector<Ghost*> ghosts) {
    for(auto ghost : ghosts) {
        if(Collision::AABBCollision(*pacman, *ghost)) {
            if(ghost->getMovState() != FRIGHTENED_MOVE) {
                for(auto eachghost : ghosts) eachghost->stopMovement();
                pacman->setFrame(0);  
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

int start(sf::RenderWindow &window, Sounds &sounds, TileMap &map) {
    // Load pacman
    Pacman pacman;
    // Load the enemies
    Ghost blinky(sf::Vector2f(105, 109), pacmanPath + "resources/images/blinkySprites.png", SCATTER_MOVE, HOUSE_MIDDLE, 1);
    Ghost clyde(sf::Vector2f(121, 133), pacmanPath + "resources/images/clydeSprites.png", HOUSE_MOVE, HOUSE_RIGHT, 3);
    Ghost pinky(sf::Vector2f(105, 133), pacmanPath + "resources/images/pinkySprites.png", HOUSE_MOVE, HOUSE_MIDDLE, 2);
    Ghost inky(sf::Vector2f(89, 133), pacmanPath + "resources/images/inkySprites.png", HOUSE_MOVE, HOUSE_LEFT, 4);

    ghosts = {&blinky, &clyde, &pinky, &inky};

    bool win = false;
    bool lose = false;
    bool drawEnemies = true;

    while (window.isOpen()) {
        // Clear screen
        window.clear();

        // Draw the background
        map.drawBackground();
        // Draw the food
        map.drawFood();
        // Draw the score
        map.drawScore();
        // Draw pacman
        window.draw(pacman);

        // Draw the enemies
        if(drawEnemies && timeOpen.getElapsedTime().asSeconds() > 2.5f) {
            for(auto ghost : ghosts) window.draw(*ghost);
        }

        window.display();

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed ||
                (sf::Keyboard::isKeyPressed(sf::Keyboard::CONTROL) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) ||
                (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::F4)))
                window.close();
            // Manage the pacman next movement
            if(!win && timeOpen.getElapsedTime().asSeconds() >= 5.f) {
                if (event.type == sf::Event::KeyPressed) {
                    switch(event.key.code) {
                        case sf::Keyboard::Up:
                            pacman.setNextMovement(KEYBOARD_UP);
                            break;
                        case sf::Keyboard::Down:
                            pacman.setNextMovement(KEYBOARD_DOWN);
                            break;
                        case sf::Keyboard::Left:
                            pacman.setNextMovement(KEYBOARD_LEFT);
                            break;
                        case sf::Keyboard::Right:
                            pacman.setNextMovement(KEYBOARD_RIGHT);
                            break;
                    }
                }
            }
        }
        // Move pacman in the next corner if setNextMovement(int key) is called
        pacman.inputMovement(map);

        if(int food = map.eatFood(pacman.getTilePos())) {
            sounds.playChomp();
            if(food == 2) {
                sounds.intermission.play();
                for(auto ghost : ghosts) ghost->setFrightened();
            }
        }

        // Defines actions if win the game.
        if(win) {
            if(map.win()) return PACMAN_WINS;
        } else {
            if(map.noFood()) {
                win = true;
                sounds.stopSounds();
                for(auto ghost : ghosts) {
                    ghost->enableMovement = false;
                    ghost->animation = false;
                }
                pacman.enableMovement = false;
                pacman.animation = false;
                map.winClock = new sf::Clock();
                map.draw_food = false;
            }
        }

        // Defines actions if lose the game.
        if(lose && !win) {
            if(pacman.lose()) return PACMAN_LOSE;
        } else {
            if(CollisionWithGhosts(&pacman, ghosts)) {
                lose = true;
                sounds.stopSounds();
                sf::sleep(sf::seconds(0.6f));
                drawEnemies = false;
                sounds.lose.play();
            }
        }

        // Update pacman
        if(sounds.intro.getStatus() != sf::SoundSource::Playing && timeOpen.getElapsedTime().asSeconds() >= 5.f) {
            if(sounds.intermission.getStatus() != sf::SoundSource::Playing) {
                if(!lose && !win && sounds.siren.getStatus() != sf::SoundSource::Playing) sounds.siren.play();
            } else sounds.siren.stop();

            // if(firstStart) {
            //     for(auto ghost : ghosts) ghost->restartHouseClock();
            //     firstStart = false;
            // }
            pacman.update(map);
            // Update the enemies position
            for(auto ghost : ghosts) ghost->update(map, pacman.getTilePos(), pacman.getSpeedDirection());
        }
    }

    sounds.stopSounds();

    return PACMAN_LOSE;
}

int main(int argc, char* argv[])
{
    pacmanPath = argv[0];
    pacmanPath = pacmanPath.substr(0, pacmanPath.find_last_of(PATH_SEPARATOR)+1);

    // Start the lua interpreter
    luaInterpreter = luaL_newstate();
    luaL_openlibs(luaInterpreter);

    std::string AI_FilePath = pacmanPath + "scripts/AI.lua";

    if(FILE *file = fopen(AI_FilePath.c_str(), "r"))
        fclose(file);
    else
        AI_FilePath = pacmanPath + "scripts/AI";

    // Open the scripts/AI file
    bool state = luaL_dofile(luaInterpreter, AI_FilePath.c_str());

    if(state) {
        std::cout << lua_tostring(luaInterpreter, -1) << std::endl;
        lua_pop(luaInterpreter, 1);
        exit(EXIT_FAILURE);
    }

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman");
    window.setFramerateLimit(FRAME_RATE);

    Sounds sounds;
    TileMap map(window);

    sounds.intro.play();

    while(window.isOpen()) {
        int result = start(window, sounds, map);
        if(result == PACMAN_WINS) {
            window.clear();
            map.restart();
            sf::sleep(sf::seconds(0.5f));
            timeOpen.restart();
        } else {
            sf::sleep(sf::seconds(2.0f));
        }
    }

    lua_close(luaInterpreter);

    return EXIT_SUCCESS;
}
