#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "BaseState.h"

class GameState :
    public BaseState
{
public:
    GameState(sf::RenderWindow& window);
    virtual ~GameState();

    // Inherited via BaseState
    virtual void enter() override;
    virtual void update(int elapsed) override;
    virtual void exit() override;

private:
    long mHighScore;
    long mCurrentScore;
    int mRemainingLives;
};


#endif // !GAME_STATE_H
