#include "pch.h"
#include "PlayerInputComponent.h"

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"

#include <iostream>


PlayerInputComponent::PlayerInputComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game),
    mVel(),
    mAccel()
{
    mWindow = mGame->getWindow();

    mCollider = mGame->getComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, getEntityID());
    mTransform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());

    mTransform->setPosition(sf::Vector2f((float)Constants::SCREEN_WIDTH / 2, Constants::SCREEN_HEIGHT - mCollider->getSize().y));
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
                mAccel.x = -Constants::PADDLE_ACCEL;
            }
            else if (event.key.code == sf::Keyboard::Right ||
                event.key.code == sf::Keyboard::D)
            {
                mAccel.x = Constants::PADDLE_ACCEL;
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
    if (mVel.x > 0.f)
    {
        mVel.x = std::min(mVel.x, Constants::PADDLE_VELOCITY);
    }
    else
    {
        mVel.x = std::max(mVel.x, Constants::PADDLE_VELOCITY);
    }
    
    mTransform->move(mVel * elapsed);

    if (mTransform->getPosition().x < 0)
    {
        mTransform->setPosition(sf::Vector2f(0, Constants::SCREEN_HEIGHT - mCollider->getSize().y));
    }

    if (mTransform->getPosition().x > Constants::SCREEN_WIDTH - mCollider->getSize().x)
    {
        auto collSize = mCollider->getSize();
        mTransform->setPosition(
            sf::Vector2f(Constants::SCREEN_WIDTH - collSize.x,
                Constants::SCREEN_HEIGHT - collSize.y));
    }
}
