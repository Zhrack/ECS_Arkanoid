#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class TransformComponent;

/// <summary>
/// Base class for all colliders.
/// </summary>
/// <seealso cref="BaseComponent" />
class ColliderComponent :
    public BaseComponent
{
public:
    ColliderComponent(EntityID entityID, GameState* game);
    virtual ~ColliderComponent();

protected:
    sf::RenderWindow* mWindow;
    TransformComponent* mTranform;
};


#endif // !COLLIDER_COMPONENT_H
