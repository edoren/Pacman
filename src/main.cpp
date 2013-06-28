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

            // if (event.type == sf::Event::KeyPressed) {
            // }
            if (event.key.code == sf::Keyboard::Up) {
                pacman.setAnimation(true);
                pacman.setSpeed(sf::Vector2f(0, -1));
                pacman.setPosition(pacman.getPosition() + pacman.getSpeed());
                pacman.setDirection(SPRITE_UP);
            }
            if (event.key.code == sf::Keyboard::Down){
                pacman.setAnimation(true);
                pacman.setSpeed(sf::Vector2f(0, 1));
                pacman.setPosition(pacman.getPosition() + pacman.getSpeed());
                pacman.setDirection(SPRITE_DOWN);
            }
            if (event.key.code == sf::Keyboard::Left){
                pacman.setAnimation(true);
                pacman.setSpeed(sf::Vector2f(-1, 0));
                pacman.setPosition(pacman.getPosition() + pacman.getSpeed());
                pacman.setDirection(SPRITE_LEFT);
            }
            if (event.key.code == sf::Keyboard::Right){
                pacman.setAnimation(true);
                pacman.setSpeed(sf::Vector2f(1, 0));
                pacman.setPosition(pacman.getPosition() + pacman.getSpeed());
                pacman.setDirection(SPRITE_RIGHT);
            }
        }

        // Clear screen
        window.clear();

         // Collition Module
        if(Collision::PixelPerfectTest(background, pacman, 10)){
            pacman.setPosition(pacman.getLastPos());

            pacman.setAnimation(false);
            pacman.setSpeed(sf::Vector2f(0, 0));

            std::cout << "Collision!!, Position: (" << pacman.getPosition().x << ", " << pacman.getPosition().y << ")" << std::endl;
        }

        // Animate pacman
        pacman.startAnimation();

        // Draw the background
        window.draw(background);
        // Draw pacman
        window.draw(pacman);

        pacman.setLastPos(pacman.getPosition()); // getting the last position
        pacman.setPosition(pacman.getPosition() + pacman.getSpeed());

        window.display();
    }
    return EXIT_SUCCESS;
}
