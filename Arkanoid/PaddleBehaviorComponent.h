#ifndef PLAYER_INPUT_COMPONENT_H
#define PLAYER_INPUT_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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
    STATE_DISRUPTION,
    STATE_DISABLED
};

/// <summary>
/// Implements the player controls and the paddle logic and states.
/// </summary>
/// <seealso cref="BaseComponent" />
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
    bool isDisabled() const;

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

    PaddleState mState;
    PaddleState mPreviousState;

    sf::Sound mPUSound;

    /// <summary>
    /// To measure timed effects, such as disabled blinking
    /// </summary>
    sf::Clock mClock;
    sf::Time mElapsedEffectTime;
    sf::Color mPreviousColor;
};


#endif // !PLAYER_INPUT_COMPONENT_H
