#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <functional>


// collision functions
bool AABBvsAABB(BaseComponent*, BaseComponent*, sf::Vector2f&);
bool AABBvsCircle(BaseComponent*, BaseComponent*, sf::Vector2f&);
bool CirclevsCircle(BaseComponent*, BaseComponent*, sf::Vector2f&);

/// <summary>
/// A utility class that implements a few collision detection algorithms.
/// </summary>
class CollisionDetector
{
public:
    CollisionDetector();
    ~CollisionDetector();

    void checkCollisions(const std::vector<BaseComponent*>& actors);

private:
    /// <summary>
    /// Map that stores algorithm functions to check collisions.
    /// If the need to expands arise, the pair can be swithed to (int, int) 
    /// </summary>
    std::unordered_map<
        int,
        std::function<bool(BaseComponent *, BaseComponent *, sf::Vector2f&)>> mFunctionMap;
};


#endif // !COLLISION_DETECTOR_H
