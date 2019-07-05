#ifndef GAME_OVER_WATCHER_COMPONENT_H
#define GAME_OVER_WATCHER_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Audio.hpp>

class GameOverWatcherComponent :
    public BaseComponent
{
public:
    GameOverWatcherComponent(EntityID entityID, GameState* game);
    virtual ~GameOverWatcherComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

private:
    virtual void handleMessage(Message & msg) override;

private:
    int mNumBricks;

    sf::Sound mLostBallSound;
};


#endif // !GAME_OVER_WATCHER_COMPONENT_H
