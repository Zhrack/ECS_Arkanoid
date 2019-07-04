
#include "BaseState.h"

#include "World.h"

BaseState::BaseState(World* world) :
    mWorld(world),
    mWindow(mWorld->getWindow())
{
}


BaseState::~BaseState()
{
}

sf::RenderWindow * BaseState::getWindow() const
{
    return mWindow;
}
