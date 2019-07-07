#include "PowerUpService.h"

#include <time.h>
#include <stdlib.h>

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "RectRenderComponent.h"
#include "CircleRenderComponent.h"
#include "SpriteRenderComponent.h"

#include "PowerUpStickyComponent.h"
#include "PowerUpDisruptionComponent.h"
#include "PowerUpBomberComponent.h"

#include "PCTextureService.h"
#include "ServiceLocator.h"


PowerUpService::PowerUpService(GameState* game) :
    mGame(game)
{
    srand((unsigned int)time(NULL));
}


PowerUpService::~PowerUpService()
{
}

void PowerUpService::spawnRandomPU(const sf::Vector2f & pos)
{

    int chancePU = rand() % 100;
    if (chancePU > 20) return;

    PUType type = (PUType)(rand() % (PUType::PU_COUNT));

    switch (type)
    {
    case PUType::PU_STICKY:
        createStickyPU(pos);
        break;
    case PU_DISRUPTION:
        createDisruptionPU(pos);
        break;
    case PU_BOMBER:
        createBomberPU(pos);
        break;
    default:
        break;
    }
}

void PowerUpService::createStickyPU(const sf::Vector2f & pos)
{
    auto id = mGame->createEntity(EntityType::TAG_POWER_UP);

    auto config = mGame->config();

    float radius = config.get<float>("POWER_UP_SIZE");

    sf::Vector2f finalPos(pos.x - radius, pos.y - radius);

    mGame->addComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, id, radius);
    mGame->addComponent<CircleRenderComponent>(CompType::CIRCLE_RENDER, id, radius, sf::Color::Yellow);
    mGame->addComponent<PowerUpStickyComponent>(CompType::STICKY, id, finalPos);
}

void PowerUpService::createDisruptionPU(const sf::Vector2f & pos)
{
    auto id = mGame->createEntity(EntityType::TAG_POWER_UP);

    auto config = mGame->config();

    float radius = config.get<float>("POWER_UP_SIZE");

    sf::Vector2f finalPos(pos.x - radius, pos.y - radius);

    mGame->addComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, id, radius);
    mGame->addComponent<CircleRenderComponent>(CompType::CIRCLE_RENDER, id, radius, sf::Color(20, 180, 80));
    mGame->addComponent<PowerUpDisruptionComponent>(CompType::DISRUPTION, id, finalPos);
}

void PowerUpService::createBomberPU(const sf::Vector2f & pos)
{
    auto id = mGame->createEntity(EntityType::TAG_POWER_UP);

    std::cout << "BOMBER" << std::endl;

    auto config = mGame->config();

    float radius = config.get<float>("POWER_UP_BOMBER_SIZE");

    sf::Vector2f finalPos(pos.x - radius, pos.y - radius);

    sf::Texture* tex = ServiceLocator::getTextureService()->getTexture(TextureID::TEXTURE_BOMBER);
    sf::IntRect rect = ServiceLocator::getTextureService()->getTextureRect(TextureID::TEXTURE_BOMBER);

    mGame->addComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, id, radius);
    mGame->addComponent<CircleRenderComponent>(CompType::CIRCLE_RENDER, id, radius, sf::Color::White);
    mGame->addComponent<PowerUpBomberComponent>(CompType::BOMBER, id, finalPos, tex, rect);
}
