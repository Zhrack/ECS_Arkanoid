
#include "SpriteRenderComponent.h"

#include "GameState.h"
#include "TransformComponent.h"

SpriteRenderComponent::SpriteRenderComponent(EntityID entityID, GameState * game, const sf::Texture& texture, const sf::IntRect & textureRect, const sf::Color & color) :
    RenderComponent(entityID, game)
{
    mSprite.setTexture(texture);
    mSprite.setTextureRect(textureRect);
    mSprite.setColor(color);
}

SpriteRenderComponent::~SpriteRenderComponent()
{
}

void SpriteRenderComponent::update(float elapsed)
{
    elapsed;
    // update transform
    mSprite.setPosition(mTransform->getPosition());
    mSprite.setRotation(mTransform->getRotation());
    mSprite.setScale(mTransform->getScale());

    mGame->getWindow()->draw(mSprite);
}
