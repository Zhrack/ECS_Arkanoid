#ifndef PLAYER_INPUT_COMPONENT_H
#define PLAYER_INPUT_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class BoxColliderComponent;
class TransformComponent;

enum class PaddleState
{
    STATE_NORMAL = 0,
    STATE_STICKY,
    STATE_ENLARGED,
    STATE_LASER,
    STATE_SLOW,
    STATE_DISRUPTION
};

class PaddleBehaviorComponent :
    public BaseComponent
{
public:
    PaddleBehaviorComponent(EntityID entityID, GameState* game, sf::Vector2f pos);
    virtual ~PaddleBehaviorComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;
    // Inherited via MessageHandler
    virtual void handleMessage(Message& msg) override;

    float getFriction() const;
    sf::Vector2f getCurrentVelocity() const;
    float getMaxVelocity() const;

    bool isSticky() const;

    void onFireButtonPressed();

private:
    sf::RenderWindow* mWindow;
    BoxColliderComponent* mCollider;

    sf::Vector2f mVel;
    sf::Vector2f mAccel;
    float mPaddleMaxAccel;
    sf::Vector2f mPaddleSize;
    float mPaddleMaxVel;
    float mPaddleFriction;

    bool mStickyPaddle;

    PaddleState mState;
};


#endif // !PLAYER_INPUT_COMPONENT_H
