#ifndef GAME_OVER_WATCHER_H
#define GAME_OVER_WATCHER_H

#include "BaseComponent.h"

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
};


#endif // !GAME_OVER_WATCHER_H
