#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

#include "MessageHandler.h"
#include <iostream>
#include <memory>

using EntityID = unsigned long;

class GameState;

/// <summary>
/// Base class for components.
/// </summary>
/// <seealso cref="MessageHandler" />
class BaseComponent : public MessageHandler
{
public:
    BaseComponent(EntityID entityID, GameState* game);
    virtual ~BaseComponent();

    /// <summary>
    /// Update for this component.
    /// </summary>
    virtual void update(float elapsed) = 0;

    EntityID getEntityID() const;

    bool isZombie() const;
    void setZombie();

    GameState* getGameState() const;

protected:
    EntityID mEntityID;

    GameState* mGame;

    /// <summary>
    /// Zombie flag to check if this component is slated for removal
    /// </summary>
    bool mZombie;

protected:
    // Inherited via MessageHandler
    virtual void handleMessage(Message & msg) override;
};
#endif // !BASE_COMPONENT_H


