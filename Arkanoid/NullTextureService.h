#ifndef NULL_TEXTURE_SERVICE_H
#define NULL_TEXTURE_SERVICE_H

#include "TextureService.h"

/// <summary>
/// To disable the Texture Service. Useful as a placeholder.
/// </summary>
/// <seealso cref="TextureService" />
class NullTextureService :
    public TextureService
{
public:
    NullTextureService();
    virtual ~NullTextureService();

    // Inherited via TextureService
    virtual bool loadTexture(const std::string & filename, TextureID id, const sf::IntRect & textureRect) override;
    virtual sf::Texture * getTexture(TextureID id) override;
    virtual sf::IntRect getTextureRect(TextureID id) override;
};


#endif // !NULL_TEXTURE_SERVICE_H
