#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

/// <summary>
/// Transform Compnent.
/// </summary>
/// <seealso cref="BaseComponent" />
/// <seealso cref="sf::Transformable" />
class TransformComponent :
    public BaseComponent,
    public sf::Transformable
{
public:
    TransformComponent(EntityID entityID, GameState* game);
    virtual ~TransformComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;
};


#endif // !TRANSFORM_COMPONENT_H
