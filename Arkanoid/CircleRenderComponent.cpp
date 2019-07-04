
#include "CircleRenderComponent.h"

#include "GameState.h"
#include "TransformComponent.h"

CircleRenderComponent::CircleRenderComponent(EntityID entityID, GameState * game,
    float radius, const sf::Color & fillColor,
    const sf::Color & outlineColor, float outlineTickness) :
    RenderComponent(entityID, game),
    mShape(radius)
{
    mShape.setFillColor(fillColor);
    mShape.setOutlineColor(outlineColor);
    mShape.setOutlineThickness(outlineTickness);
}

CircleRenderComponent::CircleRenderComponent(EntityID entityID, GameState * game,
    float radius, sf::Texture * const texture, const sf::IntRect & textureRect) :
    RenderComponent(entityID, game),
    mShape(radius)
{
    mShape.setTexture(texture);
    mShape.setTextureRect(textureRect);
}


CircleRenderComponent::~CircleRenderComponent()
{
}

void CircleRenderComponent::update(float elapsed)
{
    elapsed;
    // update transform
    mShape.setPosition(mTranform->getPosition());
    mShape.setRotation(mTranform->getRotation());
    mShape.setScale(mTranform->getScale());

    mGame->getWindow()->draw(mShape);
}

sf::Shape & CircleRenderComponent::getShape()
{
    return mShape;
}
