#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class TransformComponent;

class RenderComponent :
    public BaseComponent
{
public:
    RenderComponent(EntityID entityID, GameState* game);
    virtual ~RenderComponent();

protected:
    TransformComponent* mTranform;
};


#endif // !RENDER_COMPONENT_H
