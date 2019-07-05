#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

#include "MessageTypes.h"

#include <functional>

class TransformComponent;

/// <summary>
/// Base class for all colliders, it takes care of all the message handling and collision callback (provided by other components).
/// Derived classes provides the right type of data to use.
/// </summary>
/// <seealso cref="BaseComponent" />
class ColliderComponent :
    public BaseComponent
{
public:
    ColliderComponent(EntityID entityID, GameState* game);
    virtual ~ColliderComponent();

    TransformComponent* getTransform() const;

    void setOnCollision(std::function<void(const CollisionData&)>& callback);

protected:
    virtual void handleMessage(Message & msg) override;

protected:
    sf::RenderWindow* mWindow;
    TransformComponent* mTranform;

    /// <summary>
    /// Callback for collision events
    /// </summary>
    std::function<void(const CollisionData&)> mOnCollision;
};


#endif // !COLLIDER_COMPONENT_H
