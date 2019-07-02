
#include "BaseComponent.h"


BaseComponent::BaseComponent(EntityID entityID, GameState* game) :
    mEntityID(entityID),
    mGame(game),
    mZombie(false)
{
}


BaseComponent::~BaseComponent()
{
}

EntityID BaseComponent::getEntityID() const
{
    return mEntityID;
}

bool BaseComponent::isZombie() const
{
    return mZombie;
}

void BaseComponent::setZombie()
{
    mZombie = true;
}

GameState * BaseComponent::getGameState() const
{
    return mGame;
}

void BaseComponent::handleMessage(Message & msg)
{
    // this is the default implementation if not needed
    msg; // to silence C4100
}
