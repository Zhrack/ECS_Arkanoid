#include "pch.h"
#include "ColliderComponent.h"

#include "GameState.h"
#include "TransformComponent.h"

#include <iostream>

ColliderComponent::ColliderComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game)
{
    mWindow = mGame->getWindow();

    mTranform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());
}


ColliderComponent::~ColliderComponent()
{
}
