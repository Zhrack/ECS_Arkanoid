#ifndef PLAYER_INPUT_COMPONENT_H
#define PLAYER_INPUT_COMPONENT_H

#include "BaseComponent.h"
class PlayerInputComponent :
    public BaseComponent
{
public:
    PlayerInputComponent(unsigned long entityID);
    virtual ~PlayerInputComponent();

    // Inherited via BaseComponent
    virtual void update() override;
    // Inherited via MessageHandler
    virtual void handleMessage(Message & msg) override;
};


#endif // !PLAYER_INPUT_COMPONENT_H
