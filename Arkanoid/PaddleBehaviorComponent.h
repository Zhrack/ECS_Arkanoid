#ifndef PLAYER_INPUT_COMPONENT_H
#define PLAYER_INPUT_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class BoxColliderComponent;
class TransformComponent;

class PaddleBehaviorComponent :
    public BaseComponent
{
public:
    PaddleBehaviorComponent(EntityID entityID, GameState* game, sf::Vector2f pos);
    virtual ~PaddleBehaviorComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

    float getFriction() const;

    sf::Vector2f getCurrentVelocity() const;
    float getMaxVelocity() const;

private:
    sf::RenderWindow* mWindow;
    BoxColliderComponent* mCollider;
    TransformComponent* mTransform;

    sf::Vector2f mVel;
    sf::Vector2f mAccel;
    float mPaddleMaxAccel;
    sf::Vector2f mPaddleSize;
    float mPaddleMaxVel;
    float mPaddleFriction;
};


#endif // !PLAYER_INPUT_COMPONENT_H
