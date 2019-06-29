#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

#include "MessageHandler.h"
#include <iostream>
#include <memory>

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
    virtual void update() = 0;

    EntityID getEntityID() const;

protected:
    EntityID mEntityID;

    GameState* mGame;

protected:
    // Inherited via MessageHandler
    virtual void handleMessage(Message & msg) override;
};
#endif // !BASE_COMPONENT_H


