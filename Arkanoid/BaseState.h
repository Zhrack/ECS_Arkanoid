#ifndef BASE_STATE_H
#define BASE_STATE_H

#include "States.h"

#include <SFML/Graphics.hpp>

/// <summary>
/// Base class for the FSM.
/// </summary>
class BaseState
{
public:
    BaseState(sf::RenderWindow& window);
    virtual ~BaseState();
    /// <summary>
    /// Called at state entrance to setup the state.
    /// </summary>
    virtual void enter() = 0;
    /// <summary>
    /// Called every step.
    /// </summary>
    virtual void update(int elapsed) = 0;
    /// <summary>
    /// Called at state exit to clean up any work.
    /// </summary>
    virtual void exit() = 0;
protected:

    sf::RenderWindow& mWindow;
};

#endif // !BASE_STATE_H

