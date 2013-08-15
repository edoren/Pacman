#ifndef FOOD_MAP_HPP
#define FOOD_MAP_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "Config.hpp"

class FoodMap {
private:
    std::vector<std::vector<int>> foodMap;

    sf::Texture foodTexture;
    sf::Sprite food1;
    sf::Sprite food2;

    sf::RenderWindow *window;

    void drawFoodinPos(sf::Vector2f pos, sf::Sprite food);

public:
    FoodMap(sf::RenderWindow &window);

    void drawFood();
    bool eatFood(sf::Vector2f pacmanPos);
};

#endif // FOOD_MAP_HPP
