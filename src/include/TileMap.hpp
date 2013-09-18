#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <cmath>
#include <Python.h>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Sprite.hpp"
#include "Config.hpp"

class TileMap {
private:
    std::vector<std::vector<char>> tileMap;
    std::vector<std::vector<char>> foodMap;
    std::vector<std::vector<char>> foodMapBackup;

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
    TileMap(sf::RenderWindow &window);
    ~TileMap();

    bool checkCollision(sf::Vector2f tilePos, int spriteDirection);
    bool isValidTilePos(sf::Vector2f tilePos);
    bool isIntersection(sf::Vector2f tilePos);
    std::vector<sf::Vector2f> avaliablePaths(sf::Vector2f tilePos);

    void drawScore();
    void drawFood();
    bool eatFood(sf::Vector2f pacmanTilePos);
    bool noFood();
    void resetFood();
};

#endif // TILE_MAP_HPP
