#include "GameOverWatcher.h"

#include "GameState.h"


GameOverWatcher::GameOverWatcher(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game)
{
}


GameOverWatcher::~GameOverWatcher()
{
}

void GameOverWatcher::update(float elapsed)
{
    elapsed;
}

void GameOverWatcher::handleMessage(Message & msg)
{
    if (msg.mType == MSG_BALL_LOST)
    {
        auto balls = mGame->getAllEntitiesByType(EntityType::TAG_BALL);

        if (balls.empty())
        {
            std::cout << "LOST LIFE" << std::endl;
            mGame->decrementPlayerLives();
        }
    }
}
