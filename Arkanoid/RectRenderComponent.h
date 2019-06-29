#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H


#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class GameState;
class TransformComponent;

class RectRenderComponent :
    public BaseComponent
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="RectRenderComponent"/> class.
    /// </summary>
    /// <param name="entityID">The entity identifier.</param>
    /// <param name="game">The GameState pointer.</param>
    /// <param name="size">The size of the rectangle.</param>
    /// <param name="fillColor">Color of the fill.</param>
    /// <param name="outlineColor">Color of the outline.</param>
    /// <param name="outlineTickness">The outline tickness.</param>
    RectRenderComponent(EntityID entityID, GameState* game,
        const sf::Vector2f& size, const sf::Color& fillColor, 
        const sf::Color& outlineColor = sf::Color::White, float outlineTickness = 0);

    /// <summary>
    /// Initializes a new instance of the <see cref="RectRenderComponent"/> class.
    /// </summary>
    /// <param name="entityID">The entity identifier.</param>
    /// <param name="game">The GameState pointer.</param>
    /// <param name="texture">The texture pointer.</param>
    /// <param name="textureRect">The texture subrect to use.</param>
    RectRenderComponent(EntityID entityID, GameState* game,
        sf::Texture* const texture, const sf::IntRect& textureRect);
    virtual ~RectRenderComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

private:
    sf::RectangleShape mShape;
    TransformComponent* mTranform;
};


#endif // !RENDER_COMPONENT_H
