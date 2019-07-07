#ifndef TEXTURE_SERVICE_H
#define TEXTURE_SERVICE_H

#include <string>

#include "TextureIDs.h"

#include <SFML/Graphics.hpp>

using TexturePair = std::pair<sf::Texture*, sf::IntRect>;

/// <summary>
/// An interface for Texture managers (for different platforms or other cases).
/// </summary>
class TextureService
{
public:
    TextureService();
    virtual ~TextureService();

    virtual bool loadTexture(const std::string& filename, TextureID id, const sf::IntRect& textureRect) = 0;
    virtual sf::Texture* getTexture(TextureID id) = 0;
    virtual sf::IntRect getTextureRect(TextureID id) = 0;
};


#endif // !TEXTURE_SERVICE_H
