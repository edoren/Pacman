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

    sf::RenderWindow *window;

    void drawFoodinPos(sf::Vector2f pos, sf::Sprite food);

public:
    FoodMap(sf::RenderWindow &window);

    void drawFood();
    bool eatFood(sf::Vector2f pacmanPos);
};

#endif // __FOOD_MAP_H__
