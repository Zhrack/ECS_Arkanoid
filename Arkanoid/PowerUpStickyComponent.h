#ifndef POWER_UP_STICKY_H
#define POWER_UP_STICKY_H

#include "PowerUpComponent.h"

class CircleColliderComponent;

/// <summary>
/// Power Up that changes the paddle to be sticky for a limited time.
/// </summary>
/// <seealso cref="PowerUpComponent" />
class PowerUpStickyComponent :
    public PowerUpComponent
{
public:
    PowerUpStickyComponent(EntityID entityID, GameState* game, sf::Vector2f pos);
    virtual ~PowerUpStickyComponent();

    void onCollisionCb(const CollisionData& data);

private:
    CircleColliderComponent* mCollider;

    float mEffectDuration;
};


#endif // !POWER_UP_STICKY_H
