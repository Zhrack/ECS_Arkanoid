
#include "BallBehaviorComponent.h"

#include "GameState.h"
#include "CircleColliderComponent.h"
#include "TransformComponent.h"


BallBehaviorComponent::BallBehaviorComponent(EntityID entityID, GameState* game, const sf::Vector2f& velocity) :
    BaseComponent(entityID, game),
    mVelocity(velocity)
{
    mWindow = mGame->getWindow();

    mCollider = mGame->getComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, getEntityID());
    mTransform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());

    std::function<void(const CollisionData& data)> cb = std::bind(&BallBehaviorComponent::onCollisionCb, this, std::placeholders::_1);

    mCollider->setOnCollision(cb);
}


BallBehaviorComponent::~BallBehaviorComponent()
{
}

void BallBehaviorComponent::update(float elapsed)
{
    sf::Vector2i screenSize(mGame->config().get<int>("SCREEN_WIDTH"), mGame->config().get<int>("SCREEN_HEIGHT"));
    mTransform->move(mVelocity * elapsed);

    float radius = mCollider->getRadius();

    if (mTransform->getPosition().x < 0 ||
        mTransform->getPosition().x + radius > screenSize.x - mCollider->getRadius())
    {
        mVelocity.x = -mVelocity.x;
    }

    if (mTransform->getPosition().y < 0 ||
        mTransform->getPosition().y + radius > screenSize.y - mCollider->getRadius())
    {
        mVelocity.y = -mVelocity.y;
    }
}

void BallBehaviorComponent::onCollisionCb(const CollisionData & data)
{
    std::cout << "Ball collision with entity " << data.otherCollider->getEntityID() << std::endl;

    //TransformComponent* otherTransform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, data.other->getEntityID());

    mTransform->move(-data.amount);

    if (data.amount.x != 0)
    {
        // collision on the horizontal direction
        mVelocity.x = -mVelocity.x;
    }

    if (data.amount.y != 0)
    {
        // collision on the vertical direction
        mVelocity.y = -mVelocity.y;
    }
}
