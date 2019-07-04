#include "PowerUpDisruptionComponent.h"

#include "GameState.h"
#include "CircleColliderComponent.h"
#include "TransformComponent.h"
#include "CircleRenderComponent.h"

#include "BallBehaviorComponent.h"

#include <chrono>
#include <time.h>
#include <cstdlib>

PowerUpDisruptionComponent::PowerUpDisruptionComponent(EntityID entityID, GameState* game, sf::Vector2f pos) :
    PowerUpComponent(entityID, game, pos)
{
    srand((unsigned int)time(NULL));
    mCollider = mGame->getComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, getEntityID());

    std::function<void(const CollisionData& data)> cb = std::bind(&PowerUpDisruptionComponent::onCollisionCb, this, std::placeholders::_1);

    mCollider->setOnCollision(cb);
}


PowerUpDisruptionComponent::~PowerUpDisruptionComponent()
{
}

void PowerUpDisruptionComponent::onCollisionCb(const CollisionData & data)
{
    auto otherID = data.otherCollider->getEntityID();
    if (mGame->getEntityType(otherID) == EntityType::TAG_PLAYER)
    {
        auto config = mGame->config();
        auto ballList = mGame->getAllEntitiesByType(EntityType::TAG_BALL);
        if (ballList.size() != 1)
        {
            return;
        }
        auto ballTransform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, ballList[0]);
        sf::Vector2f ballPos = ballTransform->getPosition();

        // create 2 more balls in the current ball position, with random velocities

        

        for (int i = 0; i < 2; ++i)
        {
            auto ballID = mGame->createEntity(EntityType::TAG_BALL);

            float ballRadius = config.get<float>("BALL_RADIUS");
            float vel = config.get<float>("BALL_MAX_VELOCITY");

            mGame->addComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, ballID, ballRadius);
            mGame->addComponent<CircleRenderComponent>(CompType::CIRCLE_RENDER, ballID, ballRadius, sf::Color::Red);
            auto ballBehavior = mGame->addComponent<BallBehaviorComponent>(CompType::BALL_BEHAVIOR, ballID, vel, ballPos);

            float val1 = (float)(rand() % (int)vel);
            float val2 = (float)(rand() % (int)vel);
            sf::Vector2f dir(val1, val2);
            ballBehavior->setVelocity(dir);
        }


        mGame->destroyEntity(getEntityID());
    }
}
