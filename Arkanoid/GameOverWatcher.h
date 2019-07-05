#ifndef GAME_OVER_WATCHER_H
#define GAME_OVER_WATCHER_H

#include "BaseComponent.h"

#include <SFML/Audio.hpp>

class GameOverWatcher :
    public BaseComponent
{
public:
    GameOverWatcher(EntityID entityID, GameState* game);
    virtual ~GameOverWatcher();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

private:
    virtual void handleMessage(Message & msg) override;

private:
    int mNumBricks;

    sf::Sound mLostBallSound;
};


#endif // !GAME_OVER_WATCHER_H
