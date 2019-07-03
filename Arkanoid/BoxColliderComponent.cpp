
#include "BoxColliderComponent.h"

#include "TransformComponent.h"

BoxColliderComponent::BoxColliderComponent(EntityID entityID, GameState* game, const sf::Vector2f& size) :
    ColliderComponent(entityID, game),
    mShape(size)
{
    mShape.setPosition(mTranform->getPosition());
}


BoxColliderComponent::~BoxColliderComponent()
{
}

void BoxColliderComponent::update(float elapsed)
{
    elapsed;
}

void BoxColliderComponent::setSize(const sf::Vector2f & size)
{
    mShape.setSize(size);
}

const sf::Vector2f& BoxColliderComponent::getSize() const
{
    return mShape.getSize();
}

sf::FloatRect BoxColliderComponent::getBounds()
{
    return mShape.getGlobalBounds();
}
