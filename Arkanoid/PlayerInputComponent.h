#ifndef PLAYER_INPUT_COMPONENT_H
#define PLAYER_INPUT_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class PlayerInputComponent :
    public BaseComponent
{
public:
    PlayerInputComponent(EntityID entityID, GameState* game);
    virtual ~PlayerInputComponent();

    // Inherited via BaseComponent
    virtual void update() override;

private:
    sf::RenderWindow* mWindow;
};


#endif // !PLAYER_INPUT_COMPONENT_H
