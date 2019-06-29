#include "pch.h"
#include "RenderComponent.h"

#include "GameState.h"
#include "TransformComponent.h"


RenderComponent::RenderComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game),
    mShape()
{
    mTranform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());
    mShape.setFillColor(sf::Color::Green);
    mShape.setPosition(10, 10);
    mShape.setSize(sf::Vector2f(50, 50));
}


RenderComponent::~RenderComponent()
{
}

void RenderComponent::update()
{
    // update transform



    mGame->getWindow()->draw(mShape);
}
