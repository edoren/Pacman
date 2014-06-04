#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <vector>
#include <initializer_list>

struct Vector2 {
    int x;
    int y;

    Vector2(int x, int y);

    bool operator==(const Vector2& other);
};

struct Tile {
    bool collidable;

    int G;  // Distance from the start to the actual tile
    int H;  // Heuristic - Manhattan
    int F;  // G + H

    Vector2 position;

    Tile* parent;

    Tile(bool collidable, const Vector2& position);
};

class Tile2DMatrix : public std::vector<std::vector<Tile>> {
 public:
    Tile2DMatrix(std::initializer_list<std::initializer_list<bool>> init_list);
    Tile2DMatrix(std::size_t rows, std::size_t columns);

 private:
    std::size_t rows, columns;
};

Tile* AStar(Tile2DMatrix& collision_map, const Vector2& start_tile, const Vector2& target_tile);
void getRoute(Tile* tile, std::vector<Vector2>& vector);

#endif  // ASTAR_HPP
