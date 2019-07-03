
#include "BallBehaviorComponent.h"

#include "GameState.h"
#include "CircleColliderComponent.h"
#include "PaddleBehaviorComponent.h"

#include "TransformComponent.h"


BallBehaviorComponent::BallBehaviorComponent(EntityID entityID, GameState* game, float velocity) :
    BaseComponent(entityID, game),
    mVelocity(velocity, velocity),
    mMaxVelocity(velocity)
{
    mWindow = mGame->getWindow();

    mCollider = mGame->getComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, getEntityID());
    mTransform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());

    std::function<void(const CollisionData& data)> cb = std::bind(&BallBehaviorComponent::onCollisionCb, this, std::placeholders::_1);

    mCollider->setOnCollision(cb);

    mPaddleComp = mGame->getPaddleComponent();
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
    if (std::fabs(data.amount.x) > 0.01f)
    {
        // collision on the horizontal direction
        mVelocity.x = -mVelocity.x;
    }

    if (std::fabs(data.amount.y) > 0.01f)
    {
        // collision on the vertical direction
        mVelocity.y = -mVelocity.y;

        if (std::fabs(mVelocity.y) < 20.f)
        {
            mVelocity.y = -50.f;
        }
    }

    if (mGame->getEntityType(data.otherCollider->getEntityID()) == EntityType::TAG_PLAYER)
    {
        mVelocity.x += (mPaddleComp->getFriction() * mPaddleComp->getCurrentVelocity().x);
        sf::Vector2f mVelUnit = mVelocity / std::sqrtf((mVelocity.x * mVelocity.x) + (mVelocity.y * mVelocity.y));
        mVelocity = mVelUnit * mMaxVelocity;

        std::cout << "VelX: " << mVelocity.x << std::endl << "VelY: " << mVelocity.y << std::endl;
    }

}
