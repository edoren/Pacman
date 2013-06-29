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
    Collision::CreateTextureAndBitmask(BGtexture, "resources/pacman-map.png");
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
                if (event.key.code == sf::Keyboard::Up) {
                    pacman.setNextMovement(KEYBOARD_UP);
                } else if (event.key.code == sf::Keyboard::Down){
                    pacman.setNextMovement(KEYBOARD_DOWN);
                } else if (event.key.code == sf::Keyboard::Left){
                    pacman.setNextMovement(KEYBOARD_LEFT);
                } else if (event.key.code == sf::Keyboard::Right){
                    pacman.setNextMovement(KEYBOARD_RIGHT);
                }
            }
        }
        // Move pacman in the next corner if setNextMovement(int key) is called
        pacman.inputMovement(background);

        // Clear screen
        window.clear();

         // Collition Module
        if(Collision::PixelPerfectTest(background, pacman)){
            pacman.setPosition(pacman.getLastPos());

            pacman.setAnimation(false);
            pacman.setSpeed(sf::Vector2f(0, 0));

            // std::cout << "Collision!!, Position: (" << pacman.getPosition().x << ", " << pacman.getPosition().y << ")" << std::endl;
        }

        // Animate pacman
        pacman.startAnimation();

        // Draw the background
        window.draw(background);
        // Draw pacman
        window.draw(pacman);

        // Get the last position
        pacman.setLastPos(pacman.getPosition()); 

        // Move pacman
        pacman.setPosition(pacman.getPosition() + pacman.getSpeed());

        window.display();
    }

    return EXIT_SUCCESS;
}
