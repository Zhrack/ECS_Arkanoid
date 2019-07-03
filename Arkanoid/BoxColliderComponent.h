#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H

#include "ColliderComponent.h"

#include "CollisionDetector.h"

#include <SFML/Graphics.hpp>


class BoxColliderComponent :
    public ColliderComponent
{
public:
    BoxColliderComponent(EntityID entityID, GameState* game, const sf::Vector2f& size);
    virtual ~BoxColliderComponent();

    // Inherited via ColliderComponent
    virtual void update(float elapsed) override;

    void setSize(const sf::Vector2f& size);
    const sf::Vector2f& getSize() const;

    sf::FloatRect getBounds();

private:
    sf::RectangleShape mShape;

    friend CollisionDetector;
};


#endif // !BOX_COLLIDER_COMPONENT_H
