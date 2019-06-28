#include "pch.h"
#include "World.h"

#include "GameState.h"


World::World() :
    mWindow(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "Arkanoid"),
    mCurrentState(nullptr)
{
}


World::~World()
{
}

void World::initialize()
{
    // set starting state
    this->changeState(new GameState(mWindow));
}

void World::loop()
{
    this->initialize();
    
    while (mWindow.isOpen())
    {
        auto elapsed = mClock.restart().asMilliseconds();

        // update state
        mCurrentState->update(elapsed);
    }

    this->terminate();
}

void World::terminate()
{
    mCurrentState->exit();
}

void World::changeState(BaseState* newState)
{
    if (mCurrentState) {
        mCurrentState->exit();
    }

    mCurrentState.reset(newState);

    mCurrentState->enter();
}
