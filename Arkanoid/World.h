#ifndef WORLD_H
#define WORLD_H

#include "BaseState.h"

#include <memory>

#include <SFML/Graphics.hpp>

/// <summary>
/// Wrapper class for everything.
/// </summary>
class World
{
public:
    World();
    ~World();

    /// <summary>
    /// Initializes everything.
    /// </summary>
    void initialize();

    /// <summary>
    /// Loops the game, using its current state.
    /// </summary>
    void loop();

    void terminate();

    /// <summary>
    /// Changes the state.
    /// </summary>
    /// <param name="newState">The new state.</param>
    void changeState(BaseState* newState);

private:
    std::unique_ptr<BaseState> mCurrentState;

    /// <summary>
    /// The clock of the game.
    /// </summary>
    sf::Clock mClock;

    /// <summary>
    /// The SFML window of the game.
    /// </summary>
    sf::RenderWindow mWindow;
};

#endif // !WORLD_H



