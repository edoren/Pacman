#include "Collision.hpp"

namespace Collision {

    bool AABBCollision(const sf::FloatRect& obj1, const sf::FloatRect& obj2) {
        if (obj1.left < (obj2.left + obj2.width) && obj1.top < (obj2.top + obj2.height) &&
            obj2.left < (obj1.left + obj1.width) && obj2.top < (obj1.top + obj1.height))
            return true;
        else
            return false;
    }

    bool checkMapCollision(tmx::TileMap *map, const sf::Vector2f& tile_pos) {
        tmx::Layer& meta_tiles = map->GetLayer("MetaTiles");

        if (tile_pos.x < 0 || tile_pos.x >= meta_tiles.GetWidth()) return false;
        if (tile_pos.y < 0 || tile_pos.y >= meta_tiles.GetHeight()) return false;

        tmx::Layer::Tile& tile = meta_tiles.GetTile(tile_pos.x, tile_pos.y);

        if (!tile.empty() && tile.GetPropertyValue("collapsible") == "t") return true;
        else return false;
    }

    int checkFoodCollision(tmx::TileMap *map, Pacman* pacman) {
        sf::Vector2f pacman_tile = { pacman->getCollisionBox().left / 8, pacman->getCollisionBox().top / 8 };
        tmx::Layer& food_map = map->GetLayer("FoodMap");

        if (pacman_tile.x < 0 || pacman_tile.x >= food_map.GetWidth()) return 0;
        if (pacman_tile.y < 0 || pacman_tile.y >= food_map.GetHeight()) return 0;

        tmx::Layer::Tile& actual_tile = food_map.GetTile(pacman_tile.x, pacman_tile.y);

        if (!actual_tile.empty() && actual_tile.visible) {
            actual_tile.visible = false;
            const std::string& food = actual_tile.GetPropertyValue("food");
            return std::stoi(food);
        }

        return 0;
    }

}
