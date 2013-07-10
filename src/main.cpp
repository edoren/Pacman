// Standard headers
#include <iostream>

// Pacman module
#include "Pacman.hpp"


int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(224, 288), "Pacman");
    // Load background
    sf::Texture BGtexture;
    if (!Collision::CreateTextureAndBitmask(BGtexture, "resources/pacman-map.png")) return EXIT_FAILURE;
    sf::Sprite background(BGtexture);

    window.setFramerateLimit(60);

    // Load pacman
    Pacman pacman;
    
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

        // Clear screen
        window.clear();

        // Backgound Collision
        pacman.backgroundCollision(background);

        // Draw the background
        window.draw(background);
        // Draw pacman
        window.draw(pacman);

        // Update pacman
        pacman.update();

        window.display();
    }

    return EXIT_SUCCESS;
}
