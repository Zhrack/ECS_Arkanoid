#ifndef BRICK_COMPONENT_H
#define BRICK_COMPONENT_H

#include "BaseComponent.h"

class BoxColliderComponent;
class RectRenderComponent;

enum BrickType
{
    BRICK_NORMAL,
    BRICK_DOUBLE,
    BRICK_2HIT
};

class BrickBehaviorComponent :
    public BaseComponent
{
public:
    BrickBehaviorComponent(EntityID entityID, GameState* game, sf::Vector2f pos, BrickType type);
    virtual ~BrickBehaviorComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

    void onCollisionCb(const CollisionData& data);

private:
    BoxColliderComponent* mCollider;
    RectRenderComponent* mRenderer;
    int mHP;

    BrickType mType;
};


#endif // !BRICK_COMPONENT_H
