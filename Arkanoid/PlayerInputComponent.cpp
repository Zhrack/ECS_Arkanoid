#include "pch.h"
#include "PlayerInputComponent.h"

#include "GameState.h"

#include <iostream>


PlayerInputComponent::PlayerInputComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game)
{
    mWindow = mGame->getWindow();
}


PlayerInputComponent::~PlayerInputComponent()
{
}

void PlayerInputComponent::update()
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
        // left key is pressed: move our character
        std::cout << "CIAO " << std::endl;
    }
}
