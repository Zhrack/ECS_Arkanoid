#ifndef BALL_BEHAVIOR_COMPONENT_H
#define BALL_BEHAVIOR_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class TransformComponent;
class CircleColliderComponent;
class PaddleBehaviorComponent;
class BoxColliderComponent;

enum class BallState
{
    BALL_NORMAL,
    BALL_FOLLOW_PADDLE
};

class BallBehaviorComponent :
    public BaseComponent
{
public:
    BallBehaviorComponent(EntityID entityID, GameState* game, float velocity, const sf::Vector2f& pos);
    virtual ~BallBehaviorComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

    void onCollisionCb(const CollisionData& data);

    void releaseBall();
    void lockBall();

    virtual void handleMessage(Message& msg) override;

    void changeState(BallState newState);

    void setVelocity(const sf::Vector2f& vel);

private:
    sf::Vector2f mVelocity;
    float mMaxVelocity;
    sf::RenderWindow* mWindow;
    CircleColliderComponent* mCollider;

    PaddleBehaviorComponent* mPaddleBehaviorComp;
    BoxColliderComponent* mPaddleCollider;

    /// <summary>
    /// Is the ball locked to the paddle?
    /// </summary>
    bool mLocked;

    BallState mState;

    sf::Sound mHitSound;
    sf::Sound mHitBrickSound;
};


#endif // !BALL_BEHAVIOR_COMPONENT_H
