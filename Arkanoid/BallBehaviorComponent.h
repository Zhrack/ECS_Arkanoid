#ifndef BALL_BEHAVIOR_COMPONENT_H
#define BALL_BEHAVIOR_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class TransformComponent;
class CircleColliderComponent;
class PaddleBehaviorComponent;

class BallBehaviorComponent :
    public BaseComponent
{
public:
    BallBehaviorComponent(EntityID entityID, GameState* game, float velocity);
    virtual ~BallBehaviorComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

    void onCollisionCb(const CollisionData& data);

private:
    sf::Vector2f mVelocity;
    float mMaxVelocity;
    sf::RenderWindow* mWindow;
    TransformComponent* mTransform;
    CircleColliderComponent* mCollider;

    PaddleBehaviorComponent* mPaddleComp;
};


#endif // !BALL_BEHAVIOR_COMPONENT_H
