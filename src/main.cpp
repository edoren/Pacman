// Standard headers
#include <iostream>

// Pacman and Ghost module
#include "Pacman.hpp"
#include "Ghost.hpp"
// Configuration module
#include "Config.hpp"

int start(sf::RenderWindow &window) {
    // Load background
    sf::Texture BGtexture;
    if (!Collision::CreateTextureAndBitmask(BGtexture, "resources/pacman-map.png")) return EXIT_FAILURE;
    sf::Sprite background(BGtexture);

    // Load pacman
    Pacman pacman;
    // Load the enemies
    Ghost blinky(sf::Vector2f(105, 109), "resources/blinkySprites.png", SCATTER_MOVE, HOUSE_MIDDLE);
    Ghost clyde(sf::Vector2f(120, 130), "resources/clydeSprites.png", HOUSE_MOVE, HOUSE_RIGHT);
    Ghost pinky(sf::Vector2f(105, 130), "resources/pinkySprites.png", HOUSE_MOVE, HOUSE_MIDDLE);
    Ghost inky(sf::Vector2f(90, 130), "resources/inkySprites.png", HOUSE_MOVE, HOUSE_LEFT);

    bool lose = false;
    sf::Clock* loseClock;

    bool drawEnemies = true;

    while (window.isOpen()) {
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

        if(!lose) {
            if(Collision::BoundingBoxTest(pacman, blinky) or Collision::BoundingBoxTest(pacman, inky) or Collision::BoundingBoxTest(pacman, pinky) or Collision::BoundingBoxTest(pacman, clyde)) {
                lose = true;
                blinky.stopMovement();
                inky.stopMovement();
                clyde.stopMovement();
                pinky.stopMovement();
                pacman.setFrame(0);
                sf::Clock loseClk;
                loseClock = &loseClk;
            }
        } else {
            if(loseClock->getElapsedTime().asSeconds() > 0.4f) drawEnemies = false;
            if(pacman.lose()) return EXIT_SUCCESS;
        }

        // Clear screen
        window.clear();

        // Update pacman
        pacman.update(background);
        // Update the enemies position
        if(drawEnemies) {
            blinky.update(background);
            inky.update(background);
            clyde.update(background);
            pinky.update(background);
        }
        
        // Draw the background
        window.draw(background);
        // Draw pacman
        window.draw(pacman);
        // Draw the enemies

        if(drawEnemies) {
            window.draw(blinky);
            window.draw(inky);
            window.draw(clyde);
            window.draw(pinky);
        }

        window.display();
    }

    return EXIT_SUCCESS;
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman");
    window.setFramerateLimit(FRAME_RATE);

    while(window.isOpen()) {
        start(window);
        sf::sleep(sf::seconds(2.0f));
    }

    return EXIT_SUCCESS;
}
