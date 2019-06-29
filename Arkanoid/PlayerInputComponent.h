#ifndef PLAYER_INPUT_COMPONENT_H
#define PLAYER_INPUT_COMPONENT_H

#include "BaseComponent.h"
class PlayerInputComponent :
    public BaseComponent
{
public:
    PlayerInputComponent(EntityID entityID, GameState* game);
    virtual ~PlayerInputComponent();

    // Inherited via BaseComponent
    virtual void update() override;
};


#endif // !PLAYER_INPUT_COMPONENT_H
