#ifndef PLAYER_INPUT_COMPONENT_H
#define PLAYER_INPUT_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class TransformComponent;

class PlayerInputComponent :
    public BaseComponent
{
public:
    PlayerInputComponent(EntityID entityID, GameState* game);
    virtual ~PlayerInputComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

private:
    sf::RenderWindow* mWindow;
    TransformComponent* mTranform;
};


#endif // !PLAYER_INPUT_COMPONENT_H
