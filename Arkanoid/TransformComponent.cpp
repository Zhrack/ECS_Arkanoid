#include "pch.h"
#include "TransformComponent.h"


TransformComponent::TransformComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game),
    mShape()
{
}


TransformComponent::~TransformComponent()
{
}

void TransformComponent::update()
{
    // do nothing
}


sf::RectangleShape TransformComponent::getShape() const
{
    return mShape;
}

void TransformComponent::setPosition(sf::Vector2f pos)
{
    mShape.setPosition(pos);
}

void TransformComponent::setRotation(float angle)
{
    mShape.setRotation(angle);
}

void TransformComponent::setScale(sf::Vector2f pos)
{
    mShape.setScale(pos);
}

const sf::Vector2f TransformComponent::getPosition() const
{
    return mShape.getPosition();
}

float TransformComponent::getRotation() const
{
    return mShape.getRotation();
}

const sf::Vector2f TransformComponent::getScale() const
{
    return mShape.getScale();
}

void TransformComponent::move(sf::Vector2f vec)
{
    mShape.move(vec);
}

void TransformComponent::rotate(float angle)
{
    mShape.rotate(angle);
}

void TransformComponent::scale(sf::Vector2f vec)
{
    mShape.scale(vec);
}
