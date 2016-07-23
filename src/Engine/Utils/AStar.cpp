#include "Engine/Utils/AStar.hpp"

#include <list>
#include <algorithm>

#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////
// Vector2 Implementation
////////////////////////////////////////////////////////////////////////////////////////

Vector2::Vector2(int x, int y) : x(x), y(y) {};

bool Vector2::operator==(const Vector2& other) {
    return (x == other.x && y == other.y);
}

////////////////////////////////////////////////////////////////////////////////////////
// Tile Implementation
////////////////////////////////////////////////////////////////////////////////////////

Tile::Tile(bool collidable, const Vector2& position)
      : collidable(collidable),
        G(0),
        H(0),
        F(0),
        position(position),
        parent(nullptr) {
}

////////////////////////////////////////////////////////////////////////////////////////
// Tile2DMatrix Implementation
////////////////////////////////////////////////////////////////////////////////////////

Tile2DMatrix::Tile2DMatrix(std::initializer_list<std::initializer_list<bool>> init_list)
      : std::vector<std::vector<Tile>>(init_list.size()),
        rows(init_list.size()) {
    unsigned int x = 0, y = 0;
    for (auto& rows : init_list) {
        this->at(y).reserve(rows.size());
        for (auto& element : rows) {
            this->at(y).emplace_back(element, Vector2(x, y));
            x++;
        }
        x = 0;
        y++;
    }
    if (!this->empty())
        columns = this->at(0).size();
    else
        columns = 0;
}

Tile2DMatrix::Tile2DMatrix(std::size_t rows, std::size_t columns)
      : std::vector<std::vector<Tile>>(rows),
        rows(rows),
        columns(columns) {
    for (unsigned int y = 0; y < rows; y++) {
        this->at(y).reserve(columns);
        for (unsigned int x = 0; x < columns; x++) {
            this->at(y).emplace_back(0, Vector2(x, y));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// AStar Implementation - http://www.policyalmanac.org/games/aStarTutorial.htm
////////////////////////////////////////////////////////////////////////////////////////

Tile* AStar(Tile2DMatrix& map, const Vector2& start_tile, const Vector2& target_tile) {
    std::list<Tile*> open_list;
    std::list<Tile*> closed_list;

    open_list.push_back(&map[start_tile.y][start_tile.x]);

    while (open_list.size()) {
        // a) Look for the lowest F cost square on the open list. We refer to this as the current square.
        auto lowest_F = open_list.begin();  // Iterator to the Tile with lowest F
        for (auto it = open_list.begin(); it != open_list.end(); it++) {
            if ((*it)->F < (*lowest_F)->F)
                (*lowest_F)->F = (*it)->F;
        }

        // b) Switch it to the closed list.
        closed_list.splice(closed_list.begin(), open_list, lowest_F);
        Tile* current_tile = closed_list.front();

        // c) For each of the 4 tiles adjacent to this tile
        std::vector<Tile*> tiles_to_check;
        if (current_tile->position.y < static_cast<int>(map.size() - 1))
            tiles_to_check.push_back( &map[current_tile->position.y + 1][current_tile->position.x] );  // Up tile
        if (current_tile->position.y > 0)
            tiles_to_check.push_back( &map[current_tile->position.y - 1][current_tile->position.x] );  // Down tile
        if (current_tile->position.x < static_cast<int>(map[0].size() - 1))
            tiles_to_check.push_back( &map[current_tile->position.y][current_tile->position.x + 1] );  // Right tile
        if (current_tile->position.x > 0)
            tiles_to_check.push_back( &map[current_tile->position.y][current_tile->position.x - 1] );  // Left tile

        for (auto& tile : tiles_to_check) {
            // If it is not walkable or if it is on the closed list, ignore it.
            if ( tile->collidable || std::find(closed_list.begin(), closed_list.end(), tile) != closed_list.end() )
                continue;

            // If it isn’t on the open list, add it to the open list.
            // Make the current tile the parent of this tile. Record the F, G, and H costs of the tile.
            if ( std::find(open_list.begin(), open_list.end(), tile) == open_list.end() ) {
                tile->G = current_tile->G + 1;
                tile->H = std::fabs(target_tile.x - tile->position.x) + std::fabs(target_tile.y - tile->position.y);
                tile->F = tile->G + tile->H;
                tile->parent = current_tile;
                open_list.push_back(tile);

                if (open_list.back()->position == target_tile)
                    return open_list.back();
            }

            // If it is on the open list already, check to see if this path to that tile is better, using G cost as the measure.
            // A lower G cost means that this is a better path.
            // If so, change the parent of the tile to the current tile, and recalculate the G and F scores of the tile.
            else {
                if (tile->G < current_tile->G) {
                    tile->G = current_tile->G + 1;
                    tile->F = tile->G + tile->H;
                    tile->parent = current_tile;
                }
            }
        }
    }

    return nullptr;
}

void getRoute(Tile* tile, std::vector<Vector2>& vector) {
    if (tile == nullptr) return;
    vector.push_back(tile->position);
    getRoute(tile->parent, vector);
}
