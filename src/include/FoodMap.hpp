#ifndef FOOD_MAP_HPP
#define FOOD_MAP_HPP

#include <Python.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Config.hpp"

class FoodMap {
private:
    std::vector<std::vector<int>> foodMap;
    std::vector<std::vector<int>> foodMapBackup;

    PyObject *scoreModule, *Calculate, *GetScore;
    std::string scriptsPath;

    sf::Texture numbersTexture;
    sf::Sprite numbers;
    sf::Texture foodTexture;
    sf::Sprite food1;
    sf::Sprite food2;

    sf::RenderWindow *window;

    void drawInPos(sf::Vector2f pos, sf::Sprite food);

public:
    FoodMap(sf::RenderWindow &window);
    ~FoodMap();

    void drawScore();
    void drawFood();
    bool eatFood(sf::Vector2f pacmanPos);
    bool noFood();
    void resetFood();
};

#endif // FOOD_MAP_HPP
