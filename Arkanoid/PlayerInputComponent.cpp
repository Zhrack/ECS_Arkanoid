#include "pch.h"
#include "PlayerInputComponent.h"

#include "GameState.h"
#include "TransformComponent.h"

#include <iostream>


PlayerInputComponent::PlayerInputComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game)
{
    mWindow = mGame->getWindow();

    mTranform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());
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
        mTranform->move(sf::Vector2f(-Constants::PADDLE_VELOCITY, 0) * elapsed);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        mTranform->move(sf::Vector2f(Constants::PADDLE_VELOCITY, 0) * elapsed);
    }
}
