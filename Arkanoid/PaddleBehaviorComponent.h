#ifndef PLAYER_INPUT_COMPONENT_H
#define PLAYER_INPUT_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class BoxColliderComponent;
class TransformComponent;
class RectRenderComponent;

enum class PaddleState
{
    STATE_NORMAL = 0,
    STATE_START,
    STATE_STICKY,
    STATE_ENLARGED,
    STATE_LASER,
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
    bool isStarting() const;
    bool isNormal() const;

    void changeState(PaddleState newState);

    void onFireButtonPressed();

private:
    sf::RenderWindow* mWindow;
    BoxColliderComponent* mCollider;
    RectRenderComponent* mRenderer;

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
