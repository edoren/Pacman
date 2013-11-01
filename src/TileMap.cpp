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
    // Load the function calculate(foodType)
    SetScore = PyObject_GetAttrString(scoreModule, "setScore");
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

    lifes = 3;

    foodAmount = 0;
    for(auto row : foodMap)
        for(auto food : row)
            if(food != 0) foodAmount += 1;

    this->draw_food = true;
    this->win_status = true;
    this->win_lastTime = 0.f;

    this->foodMapBackup = this->foodMap;
    this->window = &window;
    if (!this->BGtexture.loadFromFile(pacmanPath + "resources/images/map.png")) exit(EXIT_FAILURE);
    this->background.setTexture(BGtexture);
    this->background.setTextureRect(sf::IntRect(0, 0, 224, 288));
    if(!this->numbersTexture.loadFromFile(pacmanPath + "resources/images/numbers.png")) exit(EXIT_FAILURE);
    this->numbers.setTexture(numbersTexture);
    if(!this->mapObjects.loadFromFile(pacmanPath + "resources/images/mapObjects.png")) exit(EXIT_FAILURE);
    this->lifeSprite.setTexture(mapObjects);
    this->lifeSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    if(!this->foodTexture.loadFromFile(pacmanPath + "resources/images/food.png")) exit(EXIT_FAILURE);
    this->food1.setTexture(foodTexture);
    this->food2.setTexture(foodTexture);
    this->food1.setTextureRect(sf::IntRect(8, 0, 8, 8));
    this->food2.setTextureRect(sf::IntRect(0, 0, 8, 8));
}

TileMap::~TileMap() {
    Py_Finalize();
}

std::vector<std::vector<int>>& TileMap::getTileMap(){
    return tileMap;
}

void TileMap::drawBackground() {
    this->window->draw(this->background);
}

void TileMap::drawScore() {
    // Get the value that return GetScore and convert it to char*
    // std::string score = std::to_string(PyLong_AsLong(PyObject_GetAttrString(scoreModule, "score")));
    std::string score = PyUnicode_AsUTF8(PyObject_CallFunctionObjArgs(GetScore, NULL));
    int cont = 1;
    for(auto num : score) {
        num = num - '0';
        this->numbers.setTextureRect(sf::IntRect(10 * num, 0, 10, 10));
        this->drawInPos(sf::Vector2f(37.f + (8 * cont++), 10.f), numbers);
    }
}

void TileMap::drawFood() {
    if(this->draw_food) {
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
}

void TileMap::drawLifes() {
    for (int i = 1; i < lifes; ++i) {
        drawInPos(sf::Vector2f(16 * i, 272), lifeSprite);
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

int TileMap::getTileValue(sf::Vector2f tilePos) {
    if(isValidTilePos(tilePos) == VALID_IN_RANGE) {
        return tileMap[tilePos.y][tilePos.x];
    }
    return 0;
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

int TileMap::eatFood(sf::Vector2f pacmanTilePos) {
    if (isValidTilePos(pacmanTilePos)) {
        int posX = pacmanTilePos.x;
        int posY = pacmanTilePos.y;
        if(foodMap[posY][posX] != 0 && posY < foodMap.size() && posX < foodMap[posY].size()) {
            foodAmount -= 1;
            // Call the function calculate(foodType)
            PyObject_CallFunctionObjArgs(Calculate, PyLong_FromLong(foodMap[posY][posX]), NULL);
            int food = foodMap[posY][posX];
            foodMap[posY][posX] = 0;
            return food;
        }
    }
    return 0;
}

bool TileMap::noFood() {
    for (auto row : foodMap) {
        for (auto col : row) {
            if (col != 0) return false;
        }
    }
    return true;
}

int TileMap::getFoodAmount() {
    return foodAmount;
}

void TileMap::resetFood() {
    this->foodMap = this->foodMapBackup;
    for(auto row : foodMap)
        for(auto food : row)
            if(food != 0) foodAmount += 1;
}

bool TileMap::reduceLifes() {
    lifes -= 1;
    if(lifes != 0) return true;
    return false;
}

bool TileMap::win() {
    if(this->winClock->getElapsedTime().asSeconds() >= (this->win_lastTime + 0.14f)){
        if(this->win_status)
            this->background.setTextureRect(sf::IntRect(224, 0, 224, 288));
        else
            this->background.setTextureRect(sf::IntRect(0, 0, 224, 288));
        this->win_status = !this->win_status;
        this->win_lastTime = this->winClock->getElapsedTime().asSeconds();
    }
    if(this->winClock->getElapsedTime().asSeconds() >= 1.3f) {
        delete winClock;
        return true;
    }
    return false;
}

void TileMap::restart() {
    this->resetFood();
    this->draw_food = true;
    this->win_status = true;
    this->win_lastTime = 0.f;
    // PyObject_CallFunctionObjArgs(SetScore, PyLong_FromLong(0), NULL);
    // PyObject_SetAttrString(scoreModule, "score", PyLong_FromLong(0));
    this->background.setTextureRect(sf::IntRect(0, 0, 224, 288));
}

void sfLua::lua_pushTileMap(lua_State *L, TileMap &map) {
    lua_getglobal(luaInterpreter, "make_table");
    for (int y = 0; y < map.getTileMap().size(); ++y) {
        lua_getglobal(luaInterpreter, "make_table");
        for (int x = 0; x < map.getTileMap()[y].size(); ++x) {
            lua_createtable(L, 0, 2);
            lua_pushnumber(L, map.getTileMap()[y][x]);
            lua_setfield(L, -2, "value");
            sfLua::lua_pushVector2(luaInterpreter, sf::Vector2f(x+1, y+1));
            lua_setfield(L, -2, "pos");
            lua_getglobal(L, "TileMT"); // Gets Tile Metatable
            lua_setmetatable(L, -2);
            lua_checkstack(luaInterpreter, 10);
        }
        lua_pcall(luaInterpreter, map.getTileMap()[y].size(), 1, 0);
    }
    lua_pcall(luaInterpreter, map.getTileMap().size(), 1, 0);
}
