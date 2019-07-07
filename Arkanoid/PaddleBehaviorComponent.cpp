
#include "PaddleBehaviorComponent.h"

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"
#include "RectRenderComponent.h"

#include "AudioService.h"
#include "ServiceLocator.h"

#include <iostream>


PaddleBehaviorComponent::PaddleBehaviorComponent(EntityID entityID, GameState* game, sf::Vector2f pos) :
    BaseComponent(entityID, game),
    mVel(0.f, 0.f),
    mAccel(0.f, 0.f),
    mState(PaddleState::STATE_START),
    mElapsedEffectTime(sf::Time::Zero),
    mPreviousColor(sf::Color::Transparent)
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

    sf::SoundBuffer* buffer = ServiceLocator::getAudioService()->getSound(SoundID::POWER_UP_PICKED);
    mPUSound.setBuffer(*buffer);
}


PaddleBehaviorComponent::~PaddleBehaviorComponent()
{
}

void PaddleBehaviorComponent::update(float elapsed)
{
    const sf::RectangleShape& walls = mGame->getWalls();

    sf::Vector2f gameAreaSize(walls.getSize());
    sf::Vector2f gameAreaPos(walls.getPosition());

    if (!isDisabled() &&
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)))
    {
        mVel.x = -mPaddleMaxVel;
    }
    else if (!isDisabled() &&
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)))
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

    if (isDisabled())
    {
        // start blinking the renderer
        mElapsedEffectTime += mClock.getElapsedTime();

        if (mElapsedEffectTime.asSeconds() >= 1.f)
        {
            mElapsedEffectTime = sf::Time::Zero;

            if (mPreviousColor == sf::Color::Transparent)
            {
                mPreviousColor = mRenderer->getShape().getFillColor();
                mRenderer->getShape().setFillColor(sf::Color::Transparent);
            }
            else
            {
                mRenderer->getShape().setFillColor(mPreviousColor);
                mPreviousColor = sf::Color::Transparent;
            }
        }
        //float
    }
}

void PaddleBehaviorComponent::handleMessage(Message & msg)
{
    if (msg.mType == MessageType::MSG_PU_STICKY)
    {
        std::shared_ptr<StickyData> extraInfo = std::dynamic_pointer_cast<StickyData>(msg.mExtraInfo);

        if (extraInfo->start)
        {
            std::cout << "STICKY START " << std::endl;
            // start effect
            changeState(PaddleState::STATE_STICKY);

            mPUSound.play();

            mRenderer->getShape().setFillColor(sf::Color::Cyan);
        }
        // if not sticky, this PU has been overwritten and this is an obsolete message, so I can ignore it
        else if(!extraInfo->start && isSticky()) 
        {
            std::cout << "STICKY END " << std::endl;
            changeState(PaddleState::STATE_NORMAL);
            //mRenderer->getShape().setFillColor(sf::Color::Green);

            // tell balls to go
            Message msgBall(mEntityID, MessageType::MSG_RELEASE_BALL);
            mGame->sendMessage(EntityType::TAG_BALL, CompType::BALL_BEHAVIOR, msgBall);
        }
    }
    else if (msg.mType == MessageType::MSG_PU_DISRUPTION)
    {
        mPUSound.play();
    }
    else if (msg.mType == MessageType::MSG_PU_BOMBER)
    {
        std::shared_ptr<BomberData> extraInfo = std::dynamic_pointer_cast<BomberData>(msg.mExtraInfo);

        if (extraInfo->start)
        {
            // start effect
            std::cout << "Disable START " << std::endl;
            changeState(PaddleState::STATE_DISABLED);
            mElapsedEffectTime = sf::Time::Zero;

            mPUSound.play();
        }
        // if not disabled, this PU has been overwritten and this is an obsolete message, so I can ignore it
        else if (!extraInfo->start && isDisabled())
        {
            std::cout << "Disable END " << std::endl;
            changeState(mPreviousState);

            //if (mPreviousColor != sf::Color::Transparent)
            //{
            //    mRenderer->getShape().setFillColor(mPreviousColor);
            //    mPreviousColor = sf::Color::Transparent;
            //}
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

bool PaddleBehaviorComponent::isDisabled() const
{
    return mState == PaddleState::STATE_DISABLED;
}

void PaddleBehaviorComponent::changeState(PaddleState newState)
{
    std::string currentState, newStateString, prevState;
    switch (mState)
    {
    case PaddleState::STATE_NORMAL:
        currentState = "STATE_NORMAL";
        break;
    case PaddleState::STATE_STICKY:
        currentState = "STATE_STICKY";
        break;
    case PaddleState::STATE_DISABLED:
        currentState = "STATE_DISABLED";
        break;
    case PaddleState::STATE_START:
        currentState = "STATE_START";
        break;
    }

    switch (newState)
    {
    case PaddleState::STATE_NORMAL:
        newStateString = "STATE_NORMAL";
        break;
    case PaddleState::STATE_STICKY:
        newStateString = "STATE_STICKY";
        break;
    case PaddleState::STATE_DISABLED:
        newStateString = "STATE_DISABLED";
        break;
    case PaddleState::STATE_START:
        newStateString = "STATE_START";
        break;
    }

    switch (mPreviousState)
    {
    case PaddleState::STATE_NORMAL:
        prevState = "STATE_NORMAL";
        break;
    case PaddleState::STATE_STICKY:
        prevState = "STATE_STICKY";
        break;
    case PaddleState::STATE_DISABLED:
        prevState = "STATE_DISABLED";
        break;
    case PaddleState::STATE_START:
        prevState = "STATE_START";
        break;
    }

    std::cout << "ChangeState" << std::endl;
    std::cout << "Current " << currentState << std::endl;
    std::cout << "New " << newStateString << std::endl;
    std::cout << "Prev " << prevState << std::endl << std::endl;

    if (isDisabled() && mPreviousColor != sf::Color::Transparent)
    {
        mRenderer->getShape().setFillColor(mPreviousColor);
        mPreviousColor = sf::Color::Transparent;
    }

    if (isSticky())
    {
        mRenderer->getShape().setFillColor(sf::Color::Green);
    }

    if (!isNormal() &&
        newState == PaddleState::STATE_START)
    {
        mState = newState;
        mPreviousState = PaddleState::STATE_NORMAL;
    }

    // overwrite other powerups
    if (mState != PaddleState::STATE_NORMAL)
    {
        mPreviousState = PaddleState::STATE_NORMAL;
    }
    mPreviousState = mState;
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
