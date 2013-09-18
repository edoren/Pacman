#include "Collision.hpp"

namespace Collision
{
	// Axis Aligned Bounding Box collision
	bool AABBCollision(Sprite& object1, Sprite& object2) {
		sf::Vector2f obj1TilePos = object1.getTilePos() * TILE_SIZE;
		sf::Vector2f obj2TilePos = object2.getTilePos() * TILE_SIZE;
		sf::FloatRect tile1(obj1TilePos.x , obj1TilePos.y, TILE_SIZE, TILE_SIZE);
		sf::FloatRect tile2(obj2TilePos.x , obj2TilePos.y, TILE_SIZE, TILE_SIZE);
		return boxIntersection(tile1, tile2);
	}

	bool boxIntersection(sf::FloatRect tile1, sf::FloatRect tile2) {
	    if( (tile1.left + tile1.width) >= tile2.left && tile1.left <= (tile2.left + tile2.width) &&
	        (tile1.top + tile1.height) >= tile2.top && tile1.top <= (tile2.top + tile2.height) ) {
	        return true;
	    }
	    return false;
	}
}
