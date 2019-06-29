#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

#include "MessageHandler.h"

/// <summary>
/// Base class for components.
/// </summary>
/// <seealso cref="MessageHandler" />
class BaseComponent : public MessageHandler
{
public:
    BaseComponent(unsigned long entityID);
    virtual ~BaseComponent();

    /// <summary>
    /// Update for this component.
    /// </summary>
    virtual void update() = 0;

    inline unsigned long getEntityID() const;

protected:
    unsigned long mEntityID;

    // Inherited via MessageHandler
    virtual void handleMessage(Message & msg) override;
};
#endif // !BASE_COMPONENT_H


