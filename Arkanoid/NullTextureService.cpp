#include "NullTextureService.h"



NullTextureService::NullTextureService()
{
}


NullTextureService::~NullTextureService()
{
}

bool NullTextureService::loadTexture(const std::string & filename, TextureID id, const sf::IntRect & textureRect)
{
    filename; id; textureRect;
    return false;
}

sf::Texture * NullTextureService::getTexture(TextureID id)
{
    id;
    return nullptr;
}

sf::IntRect NullTextureService::getTextureRect(TextureID id)
{
    id;
    return sf::IntRect();
}
