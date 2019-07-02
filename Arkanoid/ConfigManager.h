#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <string>
#include <iostream>

namespace pt = boost::property_tree;

class ConfigManager
{
public:
    ConfigManager();
    ~ConfigManager();

    bool loadFile(const std::string& filename);

    template<class T>
    T get(const std::string& path);

private:
    pt::ptree mTree;
};


#endif // !CONFIG_MANAGER_H

template<class T>
inline T ConfigManager::get(const std::string & path)
{
    return mTree.get<T>(path);
}
