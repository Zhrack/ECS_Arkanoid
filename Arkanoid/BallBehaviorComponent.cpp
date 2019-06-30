#include "pch.h"
#include "BallBehaviorComponent.h"

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"


BallBehaviorComponent::BallBehaviorComponent(EntityID entityID, GameState* game, const sf::Vector2f& velocity) :
    BaseComponent(entityID, game),
    mVelocity(velocity)
{
    mWindow = mGame->getWindow();

    mCollider = mGame->getComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, getEntityID());
    mTransform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());
}


BallBehaviorComponent::~BallBehaviorComponent()
{
}

void BallBehaviorComponent::update(float elapsed)
{
    mTransform->setPosition(mTransform->getPosition() + mVelocity * elapsed);

    if (mTransform->getPosition().x < 0 ||
        mTransform->getPosition().x > Constants::SCREEN_WIDTH - mCollider->getSize().x)
    {
        mVelocity.x = -mVelocity.x;
    }

    if (mTransform->getPosition().y < 0 ||
        mTransform->getPosition().y > Constants::SCREEN_HEIGHT - mCollider->getSize().y)
    {
        mVelocity.y = -mVelocity.y;
    }
}
