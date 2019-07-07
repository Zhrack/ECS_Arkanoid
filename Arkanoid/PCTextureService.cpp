#include "PCTextureService.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

PCTextureService::PCTextureService() :
    TextureService(),
    mTextureMap()
{
}


PCTextureService::~PCTextureService()
{
    clearTextures();
}

bool PCTextureService::initialize(const std::string & textureFolder, const std::vector<TextureIDFilename>& textures)
{
    try
    {
        fs::path texturePath(textureFolder);

        if (fs::is_directory(texturePath)) {

            clearTextures();

            for (auto tup : textures)
            {
                fs::path filePath(textureFolder + std::get<1>(tup));
                if (!fs::is_regular_file(filePath))
                {
                    std::string err = "Error opening " + filePath.filename().string();
                    throw std::exception(err.c_str());
                }

                
                if (!loadTexture(textureFolder + std::get<1>(tup), std::get<0>(tup), std::get<2>(tup)))
                {
                    std::string err = "Error loading " + filePath.string();
                    throw std::exception(err.c_str());
                }
            }
        }
        else
        {
            std::string err = "Invalid texture path " + texturePath.string();
            throw std::exception(err.c_str());
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;

        return false;
    }

    return true;
}

bool PCTextureService::loadTexture(const std::string & filename, TextureID id, const sf::IntRect& textureRect)
{
    sf::Texture* tex = new sf::Texture();

    if (!tex->loadFromFile(filename, textureRect))
    {
        return false;
    }

    mTextureMap[id] = std::make_pair(tex, textureRect);

    return true;
}

sf::Texture* PCTextureService::getTexture(TextureID id)
{
    if (mTextureMap.count(id) > 0)
    {
        return mTextureMap[id].first;
    }

    return nullptr;
}

sf::IntRect PCTextureService::getTextureRect(TextureID id)
{
    if (mTextureMap.count(id) > 0)
    {
        return mTextureMap[id].second;
    }

    return sf::IntRect();
}

void PCTextureService::clearTextures()
{
    for (auto it = mTextureMap.begin(); it != mTextureMap.end();)
    {
        // deallocate the texture
        delete it->second.first;
        it = mTextureMap.erase(it);
    }
}
