#ifndef POWER_UP_H
#define POWER_UP_H

#include "BaseComponent.h"

class CircleColliderComponent;

class PowerUpComponent :
    public BaseComponent
{
public:
    PowerUpComponent(EntityID entityID, GameState* game, sf::Vector2f pos);
    virtual ~PowerUpComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

protected:
    float mVelocity;
};


#endif // !POWER_UP_H
