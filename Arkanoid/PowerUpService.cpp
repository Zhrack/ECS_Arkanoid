#include "PowerUpService.h"

#include <time.h>
#include <stdlib.h>

#include "GameState.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "RectRenderComponent.h"
#include "CircleRenderComponent.h"

#include "PowerUpStickyComponent.h"


PowerUpService::PowerUpService(GameState* game) :
    mGame(game),
    mGenerator(),
    mDistr(0, PUType::PU_COUNT - 1)
{
    srand((unsigned int)time(NULL));
}


PowerUpService::~PowerUpService()
{
}

void PowerUpService::spawnRandomPU(const sf::Vector2f & pos)
{
    PUType type = (PUType)(mDistr(mGenerator));

    switch (type)
    {
    case PUType::PU_STICKY:
        createStickyPU(pos);
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

    mGame->addComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, id, radius);
    mGame->addComponent<PowerUpStickyComponent>(CompType::STICKY, id, pos);
    mGame->addComponent<CircleRenderComponent>(CompType::CIRCLE_RENDER, id, radius, sf::Color::Yellow);
}
