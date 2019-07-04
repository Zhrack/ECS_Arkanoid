#ifndef POWER_UP_STICKY_H
#define POWER_UP_STICKY_H

#include "BaseComponent.h"

class CircleColliderComponent;

class PowerUpStickyComponent :
    public BaseComponent
{
public:
    PowerUpStickyComponent(EntityID entityID, GameState* game, sf::Vector2f pos);
    virtual ~PowerUpStickyComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

    void onCollisionCb(const CollisionData& data);

private:
    CircleColliderComponent* mCollider;

    float mVelocity;
};


#endif // !POWER_UP_STICKY_H
