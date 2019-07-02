
#include "BaseState.h"


BaseState::BaseState(sf::RenderWindow* window) :
    mWindow(window)
{
}


BaseState::~BaseState()
{
}

sf::RenderWindow * BaseState::getWindow() const
{
    return mWindow;
}
