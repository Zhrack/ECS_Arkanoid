#include "GameOverWatcherComponent.h"

#include "GameState.h"
#include "AudioService.h"
#include "ServiceLocator.h"


GameOverWatcherComponent::GameOverWatcherComponent(EntityID entityID, GameState* game) :
    BaseComponent(entityID, game)
{
    auto bricks = mGame->getAllEntitiesByType(EntityType::TAG_BRICK);
    mNumBricks = (int)bricks.size();

    sf::SoundBuffer* buffer = ServiceLocator::getAudio()->getSound(SoundID::BALL_LOST);
    mLostBallSound.setBuffer(*buffer);
}


GameOverWatcherComponent::~GameOverWatcherComponent()
{
}

void GameOverWatcherComponent::update(float elapsed)
{
    elapsed;
}

void GameOverWatcherComponent::handleMessage(Message & msg)
{
    if (msg.mType == MSG_BALL_LOST)
    {
        auto balls = mGame->getAllEntitiesByType(EntityType::TAG_BALL);

        if (mLostBallSound.getStatus() != sf::SoundSource::Status::Playing)
            mLostBallSound.play();

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
