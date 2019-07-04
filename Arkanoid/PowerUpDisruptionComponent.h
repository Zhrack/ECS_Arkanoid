#ifndef POWER_UP_DISRUPTION_H
#define POWER_UP_DISRUPTION_H

#include "PowerUpComponent.h"

#include <random>

class CircleColliderComponent;

class PowerUpDisruptionComponent :
    public PowerUpComponent
{
public:
    PowerUpDisruptionComponent(EntityID entityID, GameState* game, sf::Vector2f pos);
    virtual ~PowerUpDisruptionComponent();

    void onCollisionCb(const CollisionData& data);

private:
    CircleColliderComponent* mCollider;
};


#endif // !POWER_UP_DISRUPTION_H
