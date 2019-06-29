#include "pch.h"
#include "PlayerInputComponent.h"

#include <iostream>
#include <SFML/Window.hpp>


PlayerInputComponent::PlayerInputComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game)
{
}


PlayerInputComponent::~PlayerInputComponent()
{
}

void PlayerInputComponent::update()
{
    //this->pullMessages();


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        // left key is pressed: move our character
        std::cout << "CIAO " << std::endl;
    }
}
