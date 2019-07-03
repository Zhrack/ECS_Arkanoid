#include "PowerUpStickyComponent.h"

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"

#include "PaddleBehaviorComponent.h"

PowerUpStickyComponent::PowerUpStickyComponent(EntityID entityID, GameState* game, sf::Vector2f pos) :
    BaseComponent(entityID, game)
{
    mCollider = mGame->getComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, getEntityID());

    std::function<void(const CollisionData& data)> cb = std::bind(&PowerUpStickyComponent::onCollisionCb, this, std::placeholders::_1);

    mCollider->setOnCollision(cb);

    mTransform->setPosition(pos);

    mVelocity = mGame->config().get<float>("POWER_UP_VELOCITY");
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
        Message msg;
        msg.mSenderID = getEntityID();
        msg.mType = MessageType::MSG_PU_STICKY;
        mGame->getComponent<PaddleBehaviorComponent>(CompType::STICKY, otherID)->receive(msg);

        mGame->destroyEntity(getEntityID());
    }
}
