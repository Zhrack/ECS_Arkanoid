#include "PowerUpStickyComponent.h"

#include "GameState.h"
#include "CircleColliderComponent.h"
#include "TransformComponent.h"

#include "PaddleBehaviorComponent.h"

PowerUpStickyComponent::PowerUpStickyComponent(EntityID entityID, GameState* game, sf::Vector2f pos) :
    BaseComponent(entityID, game)
{
    mCollider = mGame->getComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, getEntityID());

    std::function<void(const CollisionData& data)> cb = std::bind(&PowerUpStickyComponent::onCollisionCb, this, std::placeholders::_1);

    mCollider->setOnCollision(cb);

    mTransform->setPosition(pos);

    mVelocity = mGame->config().get<float>("POWER_UP_VELOCITY");
    mEffectDuration = mGame->config().get<float>("POWER_UP_STICKY_TIME_SEC");
}


PowerUpStickyComponent::~PowerUpStickyComponent()
{
}

void PowerUpStickyComponent::update(float elapsed)
{
    mTransform->move(0.f, mVelocity * elapsed);

    if (mTransform->getPosition().y > mGame->config().get<float>("SCREEN_HEIGHT"))
    {
        mGame->destroyEntity(getEntityID());
    }
}

void PowerUpStickyComponent::onCollisionCb(const CollisionData & data)
{
    auto otherID = data.otherCollider->getEntityID();
    if (mGame->getEntityType(otherID) == EntityType::TAG_PLAYER)
    {
        auto paddleBehavior = mGame->getComponent<PaddleBehaviorComponent>(CompType::PADDLE_BEHAVIOR, otherID);
        Message msg(mEntityID, MessageType::MSG_PU_STICKY);
        paddleBehavior->receive(msg);

        // send delayed message to stop the effect
        Message msgEnd(mEntityID, MessageType::MSG_PU_END_EFFECT);
        paddleBehavior->receive(msgEnd, SendType::DELAYED, sf::seconds(mEffectDuration));

        mGame->destroyEntity(getEntityID());
    }
}
