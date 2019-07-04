
#include "PaddleBehaviorComponent.h"

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"
#include "RectRenderComponent.h"

#include <iostream>


PaddleBehaviorComponent::PaddleBehaviorComponent(EntityID entityID, GameState* game, sf::Vector2f pos) :
    BaseComponent(entityID, game),
    mVel(0.f, 0.f),
    mAccel(0.f, 0.f),
    mStickyPaddle(true),
    mState(PaddleState::STATE_NORMAL)
{
    mWindow = mGame->getWindow();

    mCollider = mGame->getComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, getEntityID());
    mRenderer = mGame->getComponent<RectRenderComponent>(CompType::RECT_RENDER, getEntityID());

    auto& config = mGame->config();

    mTransform->setPosition(pos);

    mPaddleMaxAccel = config.get<float>("PADDLE_MAX_ACCEL");
    mPaddleSize = sf::Vector2f(config.get<float>("PADDLE_SIZE_X"), config.get<float>("PADDLE_SIZE_Y"));
    mPaddleMaxVel = config.get<float>("PADDLE_MAX_VELOCITY");
    mPaddleFriction = config.get<float>("PADDLE_FRICTION");

    mStickyPaddle = true;
}


PaddleBehaviorComponent::~PaddleBehaviorComponent()
{
}

void PaddleBehaviorComponent::update(float elapsed)
{
    sf::Vector2i screenSize(mGame->config().get<int>("SCREEN_WIDTH"), mGame->config().get<int>("SCREEN_HEIGHT"));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        mVel.x = -mPaddleMaxVel;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        mVel.x = mPaddleMaxVel;
    }
    else
        mVel.x = 0;
    
    mTransform->move(mVel * elapsed);

    if (mTransform->getPosition().x < 0)
    {
        mTransform->setPosition(sf::Vector2f(0, screenSize.y - mCollider->getSize().y));
    }

    if (mTransform->getPosition().x > screenSize.x - mCollider->getSize().x)
    {
        auto collSize = mCollider->getSize();
        mTransform->setPosition(
            sf::Vector2f(screenSize.x - collSize.x,
                screenSize.y - collSize.y));
    }
}

void PaddleBehaviorComponent::handleMessage(Message & msg)
{

    switch (msg.mType)
    {
    case MSG_PU_STICKY:
        mStickyPaddle = true;
        mState = PaddleState::STATE_STICKY;

        mRenderer->getShape().setFillColor(sf::Color::Cyan);
        break;
    case MSG_PU_END_EFFECT:

        mState = PaddleState::STATE_NORMAL;
        mRenderer->getShape().setFillColor(sf::Color::Green);
        break;
    }
}

float PaddleBehaviorComponent::getFriction() const
{
    return mPaddleFriction;
}

sf::Vector2f PaddleBehaviorComponent::getCurrentVelocity() const
{
    return mVel;
}

float PaddleBehaviorComponent::getMaxVelocity() const
{
    return mPaddleMaxVel;
}

bool PaddleBehaviorComponent::isSticky() const
{
    return mState == PaddleState::STATE_STICKY;
}

void PaddleBehaviorComponent::onFireButtonPressed()
{
    if (mStickyPaddle)
    {
        // if not currently under the power up effect, disable it (to handle starting of game stickyness)
        if (!isSticky())
            mStickyPaddle = false;

        // release ball(s)
        Message msg(mEntityID, MessageType::MSG_RELEASE_BALL);
        mGame->sendMessage(EntityType::TAG_BALL, CompType::BALL_BEHAVIOR, msg);
    }
}
