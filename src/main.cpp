// Standard headers
#include <iostream>

// Pacman and Ghost module
#include "Pacman.hpp"
#include "Ghost.hpp"
// Configuration module
#include "Config.hpp"

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman");
    // Load background
    sf::Texture BGtexture;
    if (!Collision::CreateTextureAndBitmask(BGtexture, "resources/pacman-map.png")) return EXIT_FAILURE;
    sf::Sprite background(BGtexture);

    window.setFramerateLimit(FRAME_RATE);

    // Load pacman
    Pacman pacman;
    Ghost blinky(sf::Vector2f(105, 109), "resources/blinkySprites.png", SPRITE_LEFT);
    
    while (window.isOpen()) {
// 
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

        // Clear screen
        window.clear();

        // Backgound Collision
        pacman.backgroundCollision(background);

        // Update pacman
        pacman.update();
        blinky.update(background);
        
        // Draw the background
        window.draw(background);
        // Draw pacman
        window.draw(pacman);

        window.draw(blinky);

        window.display();
    }

    return EXIT_SUCCESS;
}
