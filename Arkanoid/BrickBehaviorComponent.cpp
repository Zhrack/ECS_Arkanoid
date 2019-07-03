
#include "BrickBehaviorComponent.h"

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"

BrickBehaviorComponent::BrickBehaviorComponent(EntityID entityID, GameState* game, sf::Vector2f pos, int hp) :
    BaseComponent(entityID, game),
    mHP(hp)
{
    mCollider = mGame->getComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, getEntityID());
    mTransform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());

    std::function<void(const CollisionData& data)> cb = std::bind(&BrickBehaviorComponent::onCollisionCb, this, std::placeholders::_1);

    mCollider->setOnCollision(cb);

    mTransform->setPosition(pos);
}


BrickBehaviorComponent::~BrickBehaviorComponent()
{
}

void BrickBehaviorComponent::update(float elapsed)
{
    elapsed;
}

void BrickBehaviorComponent::onCollisionCb(const CollisionData & data)
{
    auto otherID = data.otherCollider->getEntityID();
    if (mGame->getEntityType(otherID) == EntityType::TAG_BALL)
    {
        auto points = mGame->config().get<long>("BRICK_POINTS");

        mHP--;

        if (mHP == 0)
        {
            mGame->increaseScore(points);
            mGame->destroyEntity(mEntityID);
        }
        else
        {
            mGame->increaseScore((long)std::floor(points * 0.5f));
        }
    }
}
