#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "BaseState.h"

class MenuState :
    public BaseState
{
public:
    MenuState(World* world);
    virtual ~MenuState();

    // Inherited via BaseState
    virtual void enter() override;
    virtual void update() override;
    virtual void exit() override;
};


#endif // !MENU_STATE_H
