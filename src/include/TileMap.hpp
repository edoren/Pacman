#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <cmath>
#include <Python.h>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Sprite.hpp"
#include "Config.hpp"

#define NOT_VALID 0
#define VALID_IN_RANGE 1
#define VALID_OUT_RANGE 2

class TileMap {
private:
    std::vector<std::vector<int>> tileMap;
    std::vector<std::vector<int>> foodMap;
    std::vector<std::vector<int>> foodMapBackup;

    PyObject *scoreModule, *Calculate, *GetScore;
    std::string scriptsPath;

    sf::Texture BGtexture;
    sf::Sprite background;
    sf::Texture numbersTexture;
    sf::Sprite numbers;
    sf::Texture foodTexture;
    sf::Sprite food1;
    sf::Sprite food2;

    sf::RenderWindow *window;

    void drawInPos(sf::Vector2f pos, sf::Sprite food);
    bool isInRange(sf::Vector2f tilePos);

public:
    TileMap(sf::RenderWindow &window);
    ~TileMap();

    bool checkCollision(sf::Vector2f tilePos, int spriteDirection);
    int isValidTilePos(sf::Vector2f tilePos);
    int getTileValue(sf::Vector2f tilePos);
    bool isIntersection(sf::Vector2f tilePos);
    std::vector<sf::Vector2f> avaliablePaths(sf::Vector2f tilePos);

    void drawBackground();
    void drawScore();
    void drawFood();
    int eatFood(sf::Vector2f pacmanTilePos);
    bool noFood();
    void resetFood();
};

#endif // TILE_MAP_HPP
