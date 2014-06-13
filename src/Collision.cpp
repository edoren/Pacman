#include "Collision.hpp"

namespace Collision {

    bool AABBCollision(const sf::FloatRect& obj1, const sf::FloatRect& obj2) {
        if (obj1.left < (obj2.left + obj2.width) && obj1.top < (obj2.top + obj2.height) &&
            obj2.left < (obj1.left + obj1.width) && obj2.top < (obj1.top + obj1.height))
            return true;
        else
            return false;
    }

    bool checkMapCollision(tmx::TileMap *map, const sf::FloatRect& collision_box) {
        tmx::Layer& meta_tiles = map->GetLayer("MetaTiles");

        // Iterate over all the MetaTiles
        for (unsigned int y = 0; y < meta_tiles.GetHeight(); ++y) {
            for (unsigned int x = 0; x < meta_tiles.GetWidth(); ++x) {

                tmx::Layer::Tile& tile = meta_tiles.GetTile(x, y);

                // Check if the tile is not an empty tile and has the collapsible property in t(true),
                // if it has, checks the collision and return true if it happens
                if (!tile.empty() && tile.GetPropertyValue("collapsible") == "t" &&
                    Collision::AABBCollision(collision_box, tile.GetGlobalBounds())) {
                    return true;
                }
            }
        }

        return false;
    }

    int checkFoodCollision(tmx::TileMap *map, Pacman* pacman) {
        sf::FloatRect pacman_box = pacman->getCollisionBox();
        tmx::Layer& food_map = map->GetLayer("FoodMap");

        // Iterate over all the FoodMap tiles
        for (unsigned int y = 0; y < food_map.GetHeight(); ++y) {
            for (unsigned int x = 0; x < food_map.GetWidth(); ++x) {

                tmx::Layer::Tile& tile = food_map.GetTile(x, y);

                // If the tile is not an empty tile
                if (!tile.empty()) {
                    sf::FloatRect tile_box = tile.GetGlobalBounds();
                    // If the tile is visible, it hide it
                    if (tile.visible && pacman_box == tile_box) {
                        tile.visible = false;
                        return std::stoi(tile.GetPropertyValue("food"));
                    }
                }
            }
        }

        return false;
    }

}
