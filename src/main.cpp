// Standard headers
#include <iostream>

// Pacman and Ghost module
#include "Pacman.hpp"
#include "Ghost.hpp"

#include "Sound.hpp"
// Configuration module
#include "Config.hpp"

bool firstStart = true;
sf::Clock timeOpen;

int start(sf::RenderWindow &window, Sounds &sounds) {
    // Load background
    sf::Texture BGtexture;
    if (!Collision::CreateTextureAndBitmask(BGtexture, "resources/pacman-map.png")) return EXIT_FAILURE;
    sf::Sprite background(BGtexture);

    // Load pacman
    Pacman pacman;
    // Load the enemies
    Ghost blinky(sf::Vector2f(105, 109), "resources/blinkySprites.png", SCATTER_MOVE, HOUSE_MIDDLE);
    Ghost clyde(sf::Vector2f(121, 133), "resources/clydeSprites.png", HOUSE_MOVE, HOUSE_RIGHT);
    Ghost pinky(sf::Vector2f(105, 133), "resources/pinkySprites.png", HOUSE_MOVE, HOUSE_MIDDLE);
    Ghost inky(sf::Vector2f(89, 133), "resources/inkySprites.png", HOUSE_MOVE, HOUSE_LEFT);

    bool lose = false;
    bool drawEnemies = true;

    while (window.isOpen()) {
        // Clear screen
        window.clear();

        // Draw the background
        window.draw(background);
        // Draw pacman
        window.draw(pacman);

        // Draw the enemies
        if(drawEnemies && timeOpen.getElapsedTime().asSeconds() > 2.5f) {
            window.draw(blinky);
            window.draw(inky);
            window.draw(clyde);
            window.draw(pinky);
        }

        window.display();

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
            // Manage the pacman next movement
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
        // Move pacman in the next corner if setNextMovement(int key) is called
        pacman.inputMovement(background);

        // Defines actions if lose the game.
        if(!lose) {
            if(Collision::BoundingBoxTest(pacman, blinky) or Collision::BoundingBoxTest(pacman, inky) or Collision::BoundingBoxTest(pacman, pinky) or Collision::BoundingBoxTest(pacman, clyde)) {
                lose = true;
                blinky.stopMovement();
                inky.stopMovement();
                clyde.stopMovement();
                pinky.stopMovement();
                pacman.setFrame(0);
                sounds.stopSounds();
                sf::sleep(sf::seconds(0.4f));
                drawEnemies = false;
                sounds.lose.play();
            }
        } else {
            if(pacman.lose()) return EXIT_SUCCESS;
        }

        // Update pacman
        if(timeOpen.getElapsedTime().asSeconds() > 5.f) {
            if(firstStart) {
                blinky.restartHouseClock();
                inky.restartHouseClock();
                clyde.restartHouseClock();
                pinky.restartHouseClock();
                sounds.siren.play();
                firstStart = false;
            }
            pacman.update(background);
            // Update the enemies position
            blinky.update(background);
            inky.update(background);
            clyde.update(background);
            pinky.update(background);
        }
    }

    sounds.stopSounds();

    return EXIT_SUCCESS;
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman");
    window.setFramerateLimit(FRAME_RATE);

    Sounds sounds;

    sounds.intro.play();

    while(window.isOpen()) {
        start(window, sounds);
        sf::sleep(sf::seconds(2.0f));
        sounds.siren.play();
    }

    return EXIT_SUCCESS;
}
