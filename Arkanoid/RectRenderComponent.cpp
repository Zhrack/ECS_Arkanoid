#include "pch.h"
#include "RectRenderComponent.h"

#include "GameState.h"
#include "TransformComponent.h"

RectRenderComponent::RectRenderComponent(EntityID entityID, GameState * game, 
    const sf::Vector2f & size, const sf::Color & fillColor, 
    const sf::Color & outlineColor, float outlineTickness) :
    BaseComponent(entityID, game),
    mShape(sf::Vector2f(0, 0))
{
    mTranform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());
    mShape.setFillColor(fillColor);
    mShape.setSize(size);
    mShape.setOutlineColor(outlineColor);
    mShape.setOutlineThickness(outlineTickness);
}

RectRenderComponent::RectRenderComponent(EntityID entityID, GameState * game,
    sf::Texture * const texture, const sf::IntRect & textureRect) :
    BaseComponent(entityID, game),
    mShape(sf::Vector2f(0, 0))
{
    mTranform = mGame->getComponent<TransformComponent>(CompType::TRANSFORM, getEntityID());
    mShape.setTexture(texture);
    mShape.setTextureRect(textureRect);
}

RectRenderComponent::~RectRenderComponent()
{
}

void RectRenderComponent::update(float elapsed)
{
    elapsed;
    // update transform
    mShape.setPosition(mTranform->getPosition());
    mShape.setRotation(mTranform->getRotation());
    mShape.setScale(mTranform->getScale());

    mGame->getWindow()->draw(mShape);
}
