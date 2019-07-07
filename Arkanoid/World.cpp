
#include "World.h"

#include "GameState.h"
#include "MenuState.h"

World::World() :
    mWindow(new sf::RenderWindow()),
    mCurrentState(nullptr)
{
    try
    {
        if (!loadFile("resources/settings.json"))
        {
            throw std::exception("Error reading settings.json file!");
        }

        mWindow->create(sf::VideoMode(mTree.get<unsigned int>("SCREEN_WIDTH"),
            mTree.get<unsigned int>("SCREEN_HEIGHT")),
            "Arkanoid");
        mWindow->setVerticalSyncEnabled(true);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}


World::~World()
{
}

void World::initialize()
{
    // set starting state
    this->changeState(new MenuState(this, mTree));
}

void World::loop()
{
    this->initialize();
    
    while (mWindow->isOpen())
    {
        // update state
        mCurrentState->update();
    }

    this->terminate();
}

void World::terminate()
{
    mCurrentState->exit();
}

void World::changeState(BaseState* newState)
{
    if (mCurrentState) 
    {
        mCurrentState->exit();
    }

    mCurrentState.reset(newState);

    mCurrentState->enter();
}

bool World::loadFile(const std::string & filename)
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

sf::RenderWindow * World::getWindow() const
{
    return mWindow.get();
}
