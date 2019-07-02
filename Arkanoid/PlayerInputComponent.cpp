#include "pch.h"
#include "PlayerInputComponent.h"

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"

#include <iostream>


PlayerInputComponent::PlayerInputComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game),
    mVel(0.f, 0.f),
    mAccel(0.f, 0.f)
{
    mWindow = mGame->getWindow();

    mCollider = mGame->getComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, getEntityID());
    mTransform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());

    auto& config = mGame->config();

    sf::Vector2f pos((float)config.get<int>("SCREEN_WIDTH") / 2, (float)config.get<int>("SCREEN_HEIGHT") - mCollider->getSize().y);

    mTransform->setPosition(pos);

    mPaddleMaxAccel = config.get<float>("PADDLE_MAX_ACCEL");
    mPaddleSize = sf::Vector2f(config.get<float>("PADDLE_SIZE_X"), config.get<float>("PADDLE_SIZE_Y"));
    mPaddleMaxVel = config.get<float>("PADDLE_MAX_VELOCITY");
}


PlayerInputComponent::~PlayerInputComponent()
{
}

void PlayerInputComponent::update(float elapsed)
{
    //this->pullMessages();
    sf::Event event;
    while (mWindow->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            mWindow->close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Left ||
                event.key.code == sf::Keyboard::A)
            {
                mAccel.x = -mPaddleMaxAccel;
            }
            else if (event.key.code == sf::Keyboard::Right ||
                event.key.code == sf::Keyboard::D)
            {
                mAccel.x = mPaddleMaxAccel;
            }
            break;

        case sf::Event::KeyReleased:
            if (event.key.code == sf::Keyboard::Left ||
                event.key.code == sf::Keyboard::A ||
                event.key.code == sf::Keyboard::Right ||
                event.key.code == sf::Keyboard::D)
            {
                mAccel.x = 0;
            }
            break;
        }
    }

    sf::Vector2i screenSize(mGame->config().get<int>("SCREEN_WIDTH"), mGame->config().get<int>("SCREEN_HEIGHT"));

    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
    //    sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    //{
    //    mAccel.x = - Constants::PADDLE_ACCEL;

    //}
    //else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
    //        sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    //{
    //    mAccel.x = Constants::PADDLE_ACCEL;
    //}


    mVel.x += mAccel.x;
    //if (mAccel.x != 0.f)
    //{
    //    if (mVel.x > 0.f)
    //    {
    //        mVel.x = std::min(mVel.x, Constants::PADDLE_VELOCITY);
    //    }
    //    else
    //    {
    //        mVel.x = std::max(mVel.x, Constants::PADDLE_VELOCITY);
    //    }
    //}
    
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
