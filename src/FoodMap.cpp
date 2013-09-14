#include "FoodMap.hpp"

void CheckPyError(PyObject* object) {
    if(!object) {
        PyErr_Print();
        exit(EXIT_FAILURE);
    }
}

FoodMap::FoodMap(sf::RenderWindow &window) {
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
    if(!this->numbersTexture.loadFromFile(pacmanPath + "resources/numbers.png")) exit(EXIT_FAILURE);
    this->numbers.setTexture(numbersTexture);
    if(!this->foodTexture.loadFromFile(pacmanPath + "resources/food.png")) exit(EXIT_FAILURE);
    this->food1.setTexture(foodTexture);
    this->food2.setTexture(foodTexture);
    this->food1.setTextureRect(sf::IntRect(8, 0, 8, 8));
    this->food2.setTextureRect(sf::IntRect(0, 0, 8, 8));
}

FoodMap::~FoodMap() {
    Py_Finalize();
}

void FoodMap::drawScore() {
    // Get the value that return GetScore and convert it to char*
    std::string score = PyUnicode_AsUTF8(PyObject_CallFunctionObjArgs(GetScore, NULL));
    int cont = 1;
    for(auto num : score) {
        num = num - '0';
        this->numbers.setTextureRect(sf::IntRect(10 * num, 0, 10, 10));
        this->drawInPos(sf::Vector2f(37.f + (8 * cont++), 10.f), numbers);
    }
}

void FoodMap::drawFood() {
    for(int row = 0; row < this->foodMap.size(); ++row) {
        for (int col = 0; col < this->foodMap[row].size(); ++col) {
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

void FoodMap::drawInPos(sf::Vector2f pos, sf::Sprite food) {
    food.setPosition(pos);
    this->window->draw(food);
}

bool FoodMap::eatFood(sf::Vector2f pacmanPos) {
    pacmanPos += sf::Vector2f(7.f,7.f);
    int posX = pacmanPos.x / 8;
    int posY = pacmanPos.y / 8;
    if(pacmanPos.x / 8.f == posX+0.5f && pacmanPos.y / 8.f == posY+0.5f && foodMap[posY][posX] != 0) {
        // Call the function calculate(foodType) 
        PyObject_CallFunctionObjArgs(Calculate, PyLong_FromLong(foodMap[posY][posX]), NULL);
        foodMap[posY][posX] = 0;
        return true;
    }
    return false;
}

bool FoodMap::noFood() {
    for (auto row : foodMap) {
        for (auto col : row) {
            if (col != 0) return false;
        }
    }
    return true;
}

void FoodMap::resetFood() {
    this->foodMap = this->foodMapBackup;
}
