
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
    mState(PaddleState::STATE_START)
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
}


PaddleBehaviorComponent::~PaddleBehaviorComponent()
{
}

void PaddleBehaviorComponent::update(float elapsed)
{
    const sf::RectangleShape& walls = mGame->getWalls();

    sf::Vector2f gameAreaSize(walls.getSize());
    sf::Vector2f gameAreaPos(walls.getPosition());

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

    if (mTransform->getPosition().x < gameAreaPos.x)
    {
        mTransform->setPosition(sf::Vector2f(gameAreaPos.x, gameAreaSize.y - mCollider->getSize().y));
    }

    if (mTransform->getPosition().x > gameAreaPos.x + gameAreaSize.x - mCollider->getSize().x)
    {
        auto collSize = mCollider->getSize();
        mTransform->setPosition(
            sf::Vector2f(gameAreaPos.x + gameAreaSize.x - collSize.x,
                gameAreaSize.y - collSize.y));
    }
}

void PaddleBehaviorComponent::handleMessage(Message & msg)
{
    if (msg.mType == MSG_PU_STICKY)
    {
        std::shared_ptr<StickyData> extraInfo = std::dynamic_pointer_cast<StickyData>(msg.mExtraInfo);

        if (extraInfo->start)
        {
            // start effect
            mState = PaddleState::STATE_STICKY;

            mRenderer->getShape().setFillColor(sf::Color::Cyan);
        }
        // if not sticky, this PU has been overwritten and this is an obsolete message, so I can ignore it
        else if(!extraInfo->start && isSticky()) 
        {
            mState = PaddleState::STATE_NORMAL;
            mRenderer->getShape().setFillColor(sf::Color::Green);

            // tell balls to go
            Message msgBall(mEntityID, MessageType::MSG_RELEASE_BALL);
            mGame->sendMessage(EntityType::TAG_BALL, CompType::BALL_BEHAVIOR, msgBall);
        }
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

bool PaddleBehaviorComponent::isStarting() const
{
    return mState == PaddleState::STATE_START;
}

bool PaddleBehaviorComponent::isNormal() const
{
    return mState == PaddleState::STATE_NORMAL;
}

void PaddleBehaviorComponent::changeState(PaddleState newState)
{
    mState = newState;
}

void PaddleBehaviorComponent::onFireButtonPressed()
{
    if (isStarting() ||
        isSticky())
    {
        if(mState == PaddleState::STATE_START)
            mState = PaddleState::STATE_NORMAL;

        // release ball(s)
        Message msg(mEntityID, MessageType::MSG_RELEASE_BALL);
        mGame->sendMessage(EntityType::TAG_BALL, CompType::BALL_BEHAVIOR, msg);
    }
}
