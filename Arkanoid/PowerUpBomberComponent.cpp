#include "PowerUpBomberComponent.h"

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "TransformComponent.h"
#include "CircleRenderComponent.h"

#include "PaddleBehaviorComponent.h"

#include "AudioService.h"
#include "ServiceLocator.h"

PowerUpBomberComponent::PowerUpBomberComponent(EntityID entityID, GameState* game, sf::Vector2f pos, sf::Texture* texture, sf::IntRect textureRect) :
    PowerUpComponent(entityID, game, pos)
{
    mCollider = mGame->getComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, getEntityID());
    mRenderer = mGame->getComponent<CircleRenderComponent>(CompType::CIRCLE_RENDER, getEntityID());

    mRenderer->getShape().setTexture(texture, true);
    mRenderer->getShape().setTextureRect(textureRect);

    std::function<void(const CollisionData& data)> cb = std::bind(&PowerUpBomberComponent::onCollisionCb, this, std::placeholders::_1);
    
    mCollider->setOnCollision(cb);

    mEffectDuration = mGame->config().get<float>("POWER_UP_BOMBER_TIME_SEC");
}


PowerUpBomberComponent::~PowerUpBomberComponent()
{
}

void PowerUpBomberComponent::onCollisionCb(const CollisionData & data)
{
    auto otherID = data.otherCollider->getEntityID();
    if (mGame->getEntityType(otherID) == EntityType::TAG_PLAYER)
    {
        auto paddleBehavior = mGame->getPaddleComponent();
        std::shared_ptr<BomberData> bomberData = std::make_shared<BomberData>();
        bomberData->start = true;
        Message msg(mEntityID, MessageType::MSG_PU_BOMBER, bomberData);
        paddleBehavior->receive(msg);

        // send delayed message to stop the effect
        std::shared_ptr<BomberData> endData = std::make_shared<BomberData>();
        endData->start = false;
        Message msgEnd(mEntityID, MessageType::MSG_PU_BOMBER, endData);
        paddleBehavior->receive(msgEnd, SendType::DELAYED, sf::seconds(mEffectDuration));
        
        mGame->destroyEntity(getEntityID());
    }
}
