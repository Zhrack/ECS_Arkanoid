#ifndef PLAYER_INPUT_COMPONENT_H
#define PLAYER_INPUT_COMPONENT_H

#include "BaseComponent.h"

#include <SFML/Graphics.hpp>

class BoxColliderComponent;
class TransformComponent;

class PlayerInputComponent :
    public BaseComponent
{
public:
    PlayerInputComponent(EntityID entityID, GameState* game);
    virtual ~PlayerInputComponent();

    // Inherited via BaseComponent
    virtual void update(float elapsed) override;

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
