#ifndef BALL_BEHAVIOR_COMPONENT_H
#define BALL_BEHAVIOR_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class TransformComponent;
class BoxColliderComponent;

class BallBehaviorComponent :
    public BaseComponent
{
public:
    BallBehaviorComponent(EntityID entityID, GameState* game, const sf::Vector2f& velocity);
    virtual ~BallBehaviorComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

private:
    sf::Vector2f mVelocity;
    sf::RenderWindow* mWindow;
    TransformComponent* mTransform;
    BoxColliderComponent* mCollider;
};


#endif // !BALL_BEHAVIOR_COMPONENT_H
