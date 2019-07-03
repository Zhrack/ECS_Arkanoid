#ifndef BRICK_COMPONENT_H
#define BRICK_COMPONENT_H

#include "BaseComponent.h"

class BoxColliderComponent;

class BrickBehaviorComponent :
    public BaseComponent
{
public:
    BrickBehaviorComponent(EntityID entityID, GameState* game, sf::Vector2f pos, int hp = 1);
    virtual ~BrickBehaviorComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

    void onCollisionCb(const CollisionData& data);

private:
    BoxColliderComponent* mCollider;
    int mHP;
};


#endif // !BRICK_COMPONENT_H
