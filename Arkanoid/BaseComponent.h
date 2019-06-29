#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

#include "MessageHandler.h"

//class GameState;

/// <summary>
/// Base class for components.
/// </summary>
/// <seealso cref="MessageHandler" />
class BaseComponent : public MessageHandler
{
public:
    BaseComponent(EntityID entityID);
    virtual ~BaseComponent();

    /// <summary>
    /// Update for this component.
    /// </summary>
    virtual void update() = 0;

    EntityID getEntityID() const;

protected:
    EntityID mEntityID;

    // Inherited via MessageHandler
    virtual void handleMessage(Message & msg) override;


    //friend GameState;
};
#endif // !BASE_COMPONENT_H


