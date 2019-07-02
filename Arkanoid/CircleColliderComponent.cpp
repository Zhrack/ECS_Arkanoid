
#include "CircleColliderComponent.h"

#include "TransformComponent.h"


CircleColliderComponent::CircleColliderComponent(EntityID entityID, GameState* game, float radius) :
    ColliderComponent(entityID, game),
    mShape(radius)
{
    mShape.setPosition(mTranform->getPosition());
}


CircleColliderComponent::~CircleColliderComponent()
{
}

void CircleColliderComponent::update(float elapsed)
{
    elapsed;
}

float CircleColliderComponent::getRadius() const
{
    return mShape.getRadius();
}
