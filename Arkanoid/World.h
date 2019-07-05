#ifndef WORLD_H
#define WORLD_H

#include "BaseState.h"

#include <memory>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <SFML/Graphics.hpp>

namespace pt = boost::property_tree;

/// <summary>
/// Wrapper class for everything. 
/// It's a Finite State Machine with some basic resource management.
/// </summary>
class World
{
public:
    World();
    ~World();

    /// <summary>
    /// Initializes everything.
    /// </summary>
    void initialize();

    /// <summary>
    /// Loops the game, using its current state.
    /// </summary>
    void loop();

    /// <summary>
    /// Terminates this FSM and its current state.
    /// </summary>
    void terminate();

    /// <summary>
    /// Changes the state.
    /// </summary>
    /// <param name="newState">The new state.</param>
    void changeState(BaseState* newState);

    /// <summary>
    /// Loads settings file.
    /// </summary>
    /// <param name="filename">The filename.</param>
    /// <returns></returns>
    bool loadFile(const std::string& filename);

    sf::RenderWindow* getWindow() const;

private:
    std::unique_ptr<BaseState> mCurrentState;

    /// <summary>
    /// The SFML window of the game.
    /// </summary>
    std::unique_ptr<sf::RenderWindow> mWindow;

    /// <summary>
    /// The holding data structure for the json configuration file.
    /// </summary>
    pt::ptree mTree;
};

#endif // !WORLD_H



