#ifndef BOMBER_POWER_UP_COMPONENT_H
#define BOMBER_POWER_UP_COMPONENT_H

#include "PowerUpComponent.h"

class CircleColliderComponent;
class CircleRenderComponent;

class PowerUpBomberComponent :
    public PowerUpComponent
{
public:
    PowerUpBomberComponent(EntityID entityID, GameState* game, sf::Vector2f pos, sf::Texture* texture, sf::IntRect textureRect = sf::IntRect());
    virtual ~PowerUpBomberComponent();

    void onCollisionCb(const CollisionData& data);

private:
    CircleColliderComponent* mCollider;
    CircleRenderComponent* mRenderer;

    float mEffectDuration;
};


#endif // !BOMBER_POWER_UP_COMPONENT_H
