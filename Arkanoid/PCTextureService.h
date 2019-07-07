#ifndef PC_TEXTURE_SERVICE_H
#define PC_TEXTURE_SERVICE_H

#include "TextureService.h"

#include <unordered_map>
#include <tuple>

using TextureIDFilename = std::tuple<TextureID, std::string, sf::IntRect>;

/// <summary>
/// Implements the Texture Service on PC.
/// </summary>
/// <seealso cref="TextureService" />
class PCTextureService :
    public TextureService
{
public:
    PCTextureService();
    virtual ~PCTextureService();

    bool initialize(const std::string& textureFolder, const std::vector<TextureIDFilename>& textures);

    // Inherited via TextureService
    virtual bool loadTexture(const std::string & filename, TextureID id, const sf::IntRect& textureRect) override;
    virtual sf::Texture* getTexture(TextureID id) override;
    virtual sf::IntRect getTextureRect(TextureID id) override;

    void clearTextures();

private:
    std::unordered_map<TextureID, TexturePair> mTextureMap;
};


#endif // !PC_TEXTURE_SERVICE_H
