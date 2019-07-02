
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
    CollisionData* data = nullptr;
    switch (msg.mType)
    {
    case MSG_COLLISION:
        data = static_cast<CollisionData*>(msg.mExtraInfo);
        if(mOnCollision)
            mOnCollision(*data);
        break;
    default:
        break;
    }
}
