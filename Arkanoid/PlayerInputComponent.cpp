#include "pch.h"
#include "PlayerInputComponent.h"

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"

#include <iostream>


PlayerInputComponent::PlayerInputComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game)
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
        if (event.type == sf::Event::Closed)
            mWindow->close();

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        mTransform->move(sf::Vector2f(-Constants::PADDLE_VELOCITY, 0) * elapsed);

        if (mTransform->getPosition().x < 0)
        {
            mTransform->setPosition(sf::Vector2f(0, Constants::SCREEN_HEIGHT - mCollider->getSize().y));
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        mTransform->move(sf::Vector2f(Constants::PADDLE_VELOCITY, 0) * elapsed);

        if (mTransform->getPosition().x > Constants::SCREEN_WIDTH - mCollider->getSize().x)
        {
            mTransform->setPosition(
                sf::Vector2f(Constants::SCREEN_WIDTH - mCollider->getSize().x, 
                Constants::SCREEN_HEIGHT - mCollider->getSize().y));
        }
    }
}
