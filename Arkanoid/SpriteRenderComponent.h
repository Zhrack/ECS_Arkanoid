#ifndef SPRITE_RENDER_COMPONENT_H
#define SPRITE_RENDER_COMPONENT_H

#include "RenderComponent.h"

class GameState;

class SpriteRenderComponent :
    public RenderComponent
{
public:

    /// <summary>
    /// Initializes a new instance of the <see cref="RectRenderComponent"/> class.
    /// </summary>
    /// <param name="entityID">The entity identifier.</param>
    /// <param name="game">The GameState pointer.</param>
    /// <param name="texture">The texture reference.</param>
    /// <param name="textureRect">The texture subrect to use.</param>
    /// <param name="color">The color to add to the sprite.</param>
    SpriteRenderComponent(EntityID entityID, GameState* game,
        const sf::Texture& texture, const sf::IntRect& textureRect, const sf::Color& color = sf::Color::White);

    virtual ~SpriteRenderComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

private:
    sf::Sprite mSprite;
};


#endif // !SPRITE_RENDER_COMPONENT_H
