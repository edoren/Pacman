#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "SFML/Graphics/Rect.hpp"
#include "STP/TMXLoader.hpp"

#include "Pacman.hpp"

namespace Collision {
    bool AABBCollision(const sf::FloatRect& obj1, const sf::FloatRect& obj2);
    bool checkMapCollision(tmx::TileMap *map, const sf::Vector2f& tile_pos);
    int checkFoodCollision(tmx::TileMap *map, Pacman* pacman);
}

#endif  // COLLISION_HPP
