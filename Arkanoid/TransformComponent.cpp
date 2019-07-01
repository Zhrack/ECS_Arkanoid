#include "pch.h"
#include "TransformComponent.h"


TransformComponent::TransformComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game)
{
}


TransformComponent::~TransformComponent()
{
}

void TransformComponent::update(float elapsed)
{
    elapsed;
    // do nothing
}