
#include "PlayerInputComponent.h"

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"

#include <iostream>


PlayerInputComponent::PlayerInputComponent(EntityID entityID, GameState* game, sf::Vector2f pos) :
    BaseComponent(entityID, game),
    mVel(0.f, 0.f),
    mAccel(0.f, 0.f)
{
    mWindow = mGame->getWindow();

    mCollider = mGame->getComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, getEntityID());
    mTransform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());

    auto& config = mGame->config();

    mTransform->setPosition(pos);

    mPaddleMaxAccel = config.get<float>("PADDLE_MAX_ACCEL");
    mPaddleSize = sf::Vector2f(config.get<float>("PADDLE_SIZE_X"), config.get<float>("PADDLE_SIZE_Y"));
    mPaddleMaxVel = config.get<float>("PADDLE_MAX_VELOCITY");
    mPaddleFriction = config.get<float>("PADDLE_FRICTION");
}


PlayerInputComponent::~PlayerInputComponent()
{
}

void PlayerInputComponent::update(float elapsed)
{
    //this->pullMessages();
    

    sf::Vector2i screenSize(mGame->config().get<int>("SCREEN_WIDTH"), mGame->config().get<int>("SCREEN_HEIGHT"));

    //mVel.x += mAccel.x;

    //if (mVel.x != 0)
    //{
    //    // friction
    //    //if(mAccel.x == 0)
    //        mVel.x = mVel.x > 0 ? mVel.x - mPaddleFriction : mVel.x + mPaddleFriction;

    //    // clamp velocity
    //    if (mVel.x > mPaddleMaxVel)
    //    {
    //        mVel.x = mPaddleMaxVel;
    //    }
    //    else if (mVel.x < -mPaddleMaxVel)
    //    {
    //        mVel.x = -mPaddleMaxVel;
    //    }
    //}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        mVel.x = -mPaddleMaxVel;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        mVel.x = mPaddleMaxVel;
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
