#include "pch.h"
#include "BaseComponent.h"


BaseComponent::BaseComponent(EntityID entityID) :
    mEntityID(entityID)
{
}


BaseComponent::~BaseComponent()
{
}

EntityID BaseComponent::getEntityID() const
{
    return mEntityID;
}

void BaseComponent::handleMessage(Message & msg)
{
    // this is the default implementation if not needed
    msg; // to silence C4100
}
