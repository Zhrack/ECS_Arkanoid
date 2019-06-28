#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "BaseState.h"

/// <summary>
/// State that manages the actual game
/// </summary>
/// <seealso cref="BaseState" />
/// <seealso cref="std::enable_shared_from_this{GameState}" />
class GameState :
    public BaseState,
    public std::enable_shared_from_this<GameState>
{
public:
    GameState(sf::RenderWindow& window);
    virtual ~GameState();

    // Inherited via BaseState
    virtual void enter() override;
    virtual void update(int elapsed) override;
    virtual void exit() override;

    //TODO: createEntity, addComponent, getComponent, removeComponent

private:
    long mHighScore;
    long mCurrentScore;
    int mRemainingLives;

    /// <summary>
    /// The used to generate IDs.
    /// </summary>
    static unsigned long nextID;
};


#endif // !GAME_STATE_H
