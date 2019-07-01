#ifndef CIRCLE_RENDER_COMPONENT_H
#define CIRCLE_RENDER_COMPONENT_H

#include "RenderComponent.h"

class CircleRenderComponent :
    public RenderComponent
{
public:
    CircleRenderComponent(EntityID entityID, GameState * game,
        float radius, const sf::Color & fillColor,
        const sf::Color & outlineColor = sf::Color::White, float outlineTickness = 0);

    CircleRenderComponent(EntityID entityID, GameState * game,
        float radius, sf::Texture * const texture, const sf::IntRect & textureRect);

    virtual ~CircleRenderComponent();

    // Inherited via RenderComponent
    virtual void update(float elapsed) override;

private:
    sf::CircleShape mShape;
};


#endif // !CIRCLE_RENDER_COMPONENT_H
