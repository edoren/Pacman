#include "TileMap.hpp"

void CheckPyError(PyObject* object) {
    if(!object) {
        PyErr_Print();
        exit(EXIT_FAILURE);
    }
}

TileMap::TileMap(sf::RenderWindow &window) {
    Py_Initialize();
    scriptsPath = pacmanPath + "scripts";

    // Set Python System Path
    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString(scriptsPath.c_str()));

    // Load de module Score.py
    scoreModule = PyImport_Import(PyUnicode_FromString("Score"));
    CheckPyError(scoreModule);

    // Load the function calculate(foodType)
    Calculate = PyObject_GetAttrString(scoreModule, "calculate");
    CheckPyError(Calculate);
    // Load the function getScore()
    GetScore = PyObject_GetAttrString(scoreModule, "getScore");
    CheckPyError(GetScore);

    tileMap = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,1,1,1,1,1,2,1,1,1,1,1,1,0,0,1,1,1,1,1,1,2,1,1,1,1,1,0},
               {0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
               {0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
               {0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
               {0,2,1,1,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,1,1,1,2,0},
               {0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0},
               {0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0},
               {0,1,1,1,1,1,2,0,0,1,1,1,1,0,0,1,1,1,1,0,0,2,1,1,1,1,1,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,1,1,1,3,1,1,3,1,1,1,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,1,0,1,1,1,1,1,1,0,1,0,0,1,0,0,0,0,0,0},
               {1,1,1,1,1,1,2,1,1,2,0,1,1,1,1,1,1,0,2,1,1,2,1,1,1,1,1,1},
               {0,0,0,0,0,0,1,0,0,1,0,1,1,1,1,1,1,0,1,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,2,1,1,1,1,1,1,1,1,2,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
               {0,1,1,1,1,1,2,1,1,2,1,1,1,0,0,1,1,1,2,1,1,2,1,1,1,1,1,0},
               {0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
               {0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
               {0,1,1,1,0,0,2,1,1,2,1,1,3,1,1,3,1,1,2,1,1,2,0,0,1,1,1,0},
               {0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0},
               {0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0},
               {0,1,1,2,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,2,1,1,0},
               {0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0},
               {0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0},
               {0,1,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,1,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

    foodMap = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0},
               {0,2,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,2,0},
               {0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
               {0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
               {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
               {0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0},
               {0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0},
               {0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
               {0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0},
               {0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
               {0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
               {0,2,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,2,0},
               {0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0},
               {0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0},
               {0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0},
               {0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0},
               {0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0},
               {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
               {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

    this->foodMapBackup = this->foodMap;
    this->window = &window;
    if(!this->numbersTexture.loadFromFile(pacmanPath + "resources/images/numbers.png")) exit(EXIT_FAILURE);
    this->numbers.setTexture(numbersTexture);
    if(!this->foodTexture.loadFromFile(pacmanPath + "resources/images/food.png")) exit(EXIT_FAILURE);
    this->food1.setTexture(foodTexture);
    this->food2.setTexture(foodTexture);
    this->food1.setTextureRect(sf::IntRect(8, 0, 8, 8));
    this->food2.setTextureRect(sf::IntRect(0, 0, 8, 8));
}

TileMap::~TileMap() {
    Py_Finalize();
}

void TileMap::drawScore() {
    // Get the value that return GetScore and convert it to char*
    std::string score = PyUnicode_AsUTF8(PyObject_CallFunctionObjArgs(GetScore, NULL));
    int cont = 1;
    for(auto num : score) {
        num = num - '0';
        this->numbers.setTextureRect(sf::IntRect(10 * num, 0, 10, 10));
        this->drawInPos(sf::Vector2f(37.f + (8 * cont++), 10.f), numbers);
    }
}

void TileMap::drawFood() {
    for(unsigned int row = 0; row < this->foodMap.size(); ++row) {
        for (unsigned int col = 0; col < this->foodMap[row].size(); ++col) {
            switch(foodMap[row][col]) {
                case 1:
                    this->drawInPos(sf::Vector2f(col * 8, row * 8), food1);
                    break;
                case 2:
                    this->drawInPos(sf::Vector2f(col * 8, row * 8), food2);
                    break;
            }
        }
    }
}

void TileMap::drawInPos(sf::Vector2f pos, sf::Sprite sprite) {
    sprite.setPosition(pos);
    this->window->draw(sprite);
}

bool TileMap::isInRange(sf::Vector2f tilePos) {
    if (tilePos.x >= 0 && tilePos.x < 28 && tilePos.y >= 0 && tilePos.y < 36) return true;
    else return false;
}

int TileMap::isValidTilePos(sf::Vector2f tilePos) {
    if( (tilePos.x - (int)tilePos.x) == 0 && (tilePos.y - (int)tilePos.y) == 0 ) {
        if(isInRange(tilePos)) return VALID_IN_RANGE;
        else return VALID_OUT_RANGE;
    }
    else return NOT_VALID;
}

bool TileMap::checkCollision(sf::Vector2f tilePos, int spriteDirection) {
    if(isValidTilePos(tilePos)) {
        sf::Vector2f tileInFront = tilePos;
        switch(spriteDirection) {
            case SPRITE_UP:
                tileInFront -= sf::Vector2f(0, 1);
                break;
            case SPRITE_DOWN:
                tileInFront -= sf::Vector2f(0, -1);
                break;
            case SPRITE_LEFT:
                tileInFront -= sf::Vector2f(1, 0);
                break;
            case SPRITE_RIGHT:
                tileInFront -= sf::Vector2f(-1, 0);
                break;
        }
        if(!isInRange(tileInFront)) return false;
        if(tileMap[tileInFront.y][tileInFront.x] == 0) return true;
    } 
    return false;
}

bool TileMap::isIntersection(sf::Vector2f tilePos) {
    if(tileMap[tilePos.y][tilePos.x] == 2 || tileMap[tilePos.y][tilePos.x] == 3) return true;
    return false;
}

std::vector<sf::Vector2f> TileMap::avaliablePaths(sf::Vector2f tilePos) {
    std::vector<sf::Vector2f> avaliablePaths;
    if(isValidTilePos(tilePos)) {
        if(isValidTilePos(tilePos)) {
            if(tileMap[tilePos.y-1][tilePos.x] != 0) avaliablePaths.push_back(VECTOR_UP);
            if(tileMap[tilePos.y+1][tilePos.x] != 0) avaliablePaths.push_back(VECTOR_DOWN);
            if(tileMap[tilePos.y][tilePos.x-1] != 0) avaliablePaths.push_back(VECTOR_LEFT);
            if(tileMap[tilePos.y][tilePos.x+1] != 0) avaliablePaths.push_back(VECTOR_RIGHT);
        }
    }
    return avaliablePaths;
}

bool TileMap::eatFood(sf::Vector2f pacmanTilePos) {
    if (isValidTilePos(pacmanTilePos)) {
        int posX = pacmanTilePos.x;
        int posY = pacmanTilePos.y;
        if( foodMap[posY][posX] != 0 && posY < foodMap.size() && posX < foodMap[posY].size()) {
            // Call the function calculate(foodType)
            PyObject_CallFunctionObjArgs(Calculate, PyLong_FromLong(foodMap[posY][posX]), NULL);
            foodMap[posY][posX] = 0;
            return true;
        }
    }
    return false;
}

bool TileMap::noFood() {
    for (auto row : foodMap) {
        for (auto col : row) {
            if (col != 0) return false;
        }
    }
    return true;
}

void TileMap::resetFood() {
    this->foodMap = this->foodMapBackup;
}
