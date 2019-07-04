
#include "ColliderComponent.h"

#include "GameState.h"
#include "TransformComponent.h"

#include <iostream>

ColliderComponent::ColliderComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game),
    mOnCollision()
{
    mWindow = mGame->getWindow();

    mTranform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());
}


ColliderComponent::~ColliderComponent()
{
}

TransformComponent * ColliderComponent::getTransform() const
{
    return mTranform;
}

void ColliderComponent::setOnCollision(std::function<void(const CollisionData&)>& callback)
{
    mOnCollision = callback;
}

void ColliderComponent::handleMessage(Message & msg)
{
    if (msg.mType == MSG_COLLISION)
    {
        std::shared_ptr<CollisionData> data = std::dynamic_pointer_cast<CollisionData>(msg.mExtraInfo);
        if (mOnCollision)
            mOnCollision(*data);
    }
}
