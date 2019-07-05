
#include "BrickBehaviorComponent.h"

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"
#include "RectRenderComponent.h"

#include "PowerUpService.h"

BrickBehaviorComponent::BrickBehaviorComponent(EntityID entityID, GameState* game, sf::Vector2f pos, BrickType type) :
    BaseComponent(entityID, game),
    mType(type)
{
    mCollider = mGame->getComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, getEntityID());

    mRenderer = mGame->getComponent<RectRenderComponent>(CompType::RECT_RENDER, getEntityID());

    std::function<void(const CollisionData& data)> cb = std::bind(&BrickBehaviorComponent::onCollisionCb, this, std::placeholders::_1);

    mCollider->setOnCollision(cb);

    mTransform->setPosition(pos);

    switch (mType)
    {
    case BRICK_NORMAL:
    default:
        mHP = 1;
        mRenderer->getShape().setFillColor(sf::Color(242, 242, 242));
        break;
    case BRICK_DOUBLE:
        mHP = 1;
        mRenderer->getShape().setFillColor(sf::Color(255, 140, 8));
        break;
    case BRICK_2HIT:
        mHP = 2;
        mRenderer->getShape().setFillColor(sf::Color(189, 171, 4));
        break;
    }
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

        if (mType == BRICK_DOUBLE ||
            mType == BRICK_2HIT)
        {
            points *= 2;
        }

        mHP--;
        mGame->increaseScore(points);

        if (mHP == 0)
        {
            Message brickDestroyedMsg(getEntityID(), MessageType::MSG_BRICK_DESTROYED);
            mGame->sendMessage(EntityType::TAG_GAME_OVER_WATCHER, CompType::GAME_OVER_WATCHER, brickDestroyedMsg);
            mGame->destroyEntity(mEntityID);

            mGame->getPUService().spawnRandomPU(mTransform->getPosition() + (mCollider->getSize() / 2.f));
        }
    }
}
