#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H


#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class GameState;
class TransformComponent;

class RenderComponent :
    public BaseComponent
{
public:
    RenderComponent(EntityID entityID, GameState* game);
    virtual ~RenderComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

private:
    sf::RectangleShape mShape;
    TransformComponent* mTranform;
};


#endif // !RENDER_COMPONENT_H
