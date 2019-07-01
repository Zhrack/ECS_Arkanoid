#include "pch.h"
#include "RenderComponent.h"

#include "TransformComponent.h"
#include "GameState.h"

RenderComponent::RenderComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game)
{
    mTranform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());
}


RenderComponent::~RenderComponent()
{
}
