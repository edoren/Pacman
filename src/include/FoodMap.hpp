#ifndef __FOOD_MAP_H__
#define __FOOD_MAP_H__

#include <vector>
#include <SFML/Graphics.hpp>

class FoodMap {
private:
    std::vector<std::vector<int>> foodMap;

    sf::Texture foodTexture;
    sf::Sprite food1;
    sf::Sprite food2;

public:
    FoodMap();

    void drawFood(sf::RenderWindow &window);
    bool eatFood(sf::Vector2f pacmanPos);
};

#endif // __FOOD_MAP_H__
