#include "pch.h"
#include "ConfigManager.h"


ConfigManager::ConfigManager()
{
}


ConfigManager::~ConfigManager()
{
}

bool ConfigManager::loadFile(const std::string & filename)
{
    try
    {
        pt::read_json(filename, mTree);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }

    return true;
}
