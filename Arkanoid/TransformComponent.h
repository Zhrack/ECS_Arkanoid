#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class TransformComponent :
    public BaseComponent
{
public:
    TransformComponent(EntityID entityID, GameState* game);
    virtual ~TransformComponent();

    // Inherited via BaseComponent
    virtual void update() override;

    sf::RectangleShape getShape() const;

    void setPosition(sf::Vector2f pos);
    void setRotation(float angle);
    void setScale(sf::Vector2f pos);

    const sf::Vector2f getPosition() const;
    float getRotation() const;
    const sf::Vector2f getScale() const;

    void move(sf::Vector2f vec);
    void rotate(float angle);
    void scale(sf::Vector2f vec);

private:
    /// <summary>
    /// I only need its transform and their related functions, to draw, the rendering components use its own data with this transform.
    /// </summary>
    sf::RectangleShape mShape;
};


#endif // !TRANSFORM_COMPONENT_H
