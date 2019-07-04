#include "PowerUpComponent.h"

#include "GameState.h"
#include "CircleColliderComponent.h"
#include "TransformComponent.h"

PowerUpComponent::PowerUpComponent(EntityID entityID, GameState* game, sf::Vector2f pos) :
    BaseComponent(entityID, game)
{
    mTransform->setPosition(pos);

    mVelocity = mGame->config().get<float>("POWER_UP_VELOCITY");
}


PowerUpComponent::~PowerUpComponent()
{
}

void PowerUpComponent::update(float elapsed)
{
    mTransform->move(0.f, mVelocity * elapsed);

    if (mTransform->getPosition().y > mGame->config().get<float>("SCREEN_HEIGHT"))
    {
        mGame->destroyEntity(getEntityID());
    }
}
