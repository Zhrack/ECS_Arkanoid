#include "GameOverWatcher.h"

#include "GameState.h"


GameOverWatcher::GameOverWatcher(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game)
{
    auto bricks = mGame->getAllEntitiesByType(EntityType::TAG_BRICK);
    mNumBricks = (int)bricks.size();
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
    else if (msg.mType == MSG_BRICK_DESTROYED)
    {
        mNumBricks--;

        if (mNumBricks == 0)
        {
            // finished level
            mGame->gameOver(true);
        }
    }
}
