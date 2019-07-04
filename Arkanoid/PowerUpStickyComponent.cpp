#include "PowerUpStickyComponent.h"

#include "GameState.h"
#include "CircleColliderComponent.h"
#include "TransformComponent.h"

#include "PaddleBehaviorComponent.h"

PowerUpStickyComponent::PowerUpStickyComponent(EntityID entityID, GameState* game, sf::Vector2f pos) :
    PowerUpComponent(entityID, game, pos)
{
    mCollider = mGame->getComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, getEntityID());

    std::function<void(const CollisionData& data)> cb = std::bind(&PowerUpStickyComponent::onCollisionCb, this, std::placeholders::_1);

    mCollider->setOnCollision(cb);

    mEffectDuration = mGame->config().get<float>("POWER_UP_STICKY_TIME_SEC");
}


PowerUpStickyComponent::~PowerUpStickyComponent()
{
}

void PowerUpStickyComponent::onCollisionCb(const CollisionData & data)
{
    auto otherID = data.otherCollider->getEntityID();
    if (mGame->getEntityType(otherID) == EntityType::TAG_PLAYER)
    {
        auto paddleBehavior = mGame->getComponent<PaddleBehaviorComponent>(CompType::PADDLE_BEHAVIOR, otherID);
        std::shared_ptr<StickyData> stickyData = std::make_shared<StickyData>();
        stickyData->start = true;
        Message msg(mEntityID, MessageType::MSG_PU_STICKY, stickyData);
        paddleBehavior->receive(msg);

        // send delayed message to stop the effect
        std::shared_ptr<StickyData> endData = std::make_shared<StickyData>();
        endData->start = false;
        Message msgEnd(mEntityID, MessageType::MSG_PU_STICKY, endData);
        paddleBehavior->receive(msgEnd, SendType::DELAYED, sf::seconds(mEffectDuration));

        mGame->destroyEntity(getEntityID());
    }
}
