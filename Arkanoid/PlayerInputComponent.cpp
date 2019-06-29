#include "pch.h"
#include "PlayerInputComponent.h"

#include <iostream>
#include <SFML/Window.hpp>


PlayerInputComponent::PlayerInputComponent(unsigned long entityID) : 
    BaseComponent(entityID)
{
}


PlayerInputComponent::~PlayerInputComponent()
{
}

void PlayerInputComponent::update()
{
    this->pullMessages();
}

void PlayerInputComponent::handleMessage(Message & msg)
{
    switch (msg.mType)
    {
    case MSG_INPUT_EVENT:
        std::cout << "Received a MSG_INPUT_EVENT from entity " << msg.mSenderID << std::endl;

        std::cout << "Extra " << static_cast<sf::Event*>(msg.mExtraInfo)->type << std::endl;
        break;
    default:
        break;
    }
}
