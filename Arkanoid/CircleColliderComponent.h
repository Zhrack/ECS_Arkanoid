#ifndef CIRCLE_COLLIDER_COMPONENT_H
#define CIRCLE_COLLIDER_COMPONENT_H

#include "ColliderComponent.h"

class CircleColliderComponent :
    public ColliderComponent
{
public:
    CircleColliderComponent(EntityID entityID, GameState* game, float radius);
    virtual ~CircleColliderComponent();

    // Inherited via ColliderComponent
    virtual void update(float elapsed) override;
    
    float getRadius() const;

private:
    sf::CircleShape mShape;
};


#endif // !CIRCLE_COLLIDER_COMPONENT_H
