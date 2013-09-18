#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Sprite.hpp"

namespace Collision {
	// Axis Aligned Bounding Box collision
	bool AABBCollision(Sprite& object1, Sprite& object2);
	bool boxIntersection(sf::FloatRect tile1, sf::FloatRect tile2);
}

#endif // COLLISION_HPP
