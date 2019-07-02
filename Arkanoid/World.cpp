
#include "World.h"

#include "GameState.h"

World::World() :
    mWindow(new sf::RenderWindow()),
    mCurrentState(nullptr)
{
    if (!loadFile("settings.json"))
    {
        std::cout << "Error reading settings.json file!" << std::endl;
    }

    mWindow->create(sf::VideoMode(  mTree.get<unsigned int>("SCREEN_WIDTH"),
                                    mTree.get<unsigned int>("SCREEN_HEIGHT")), 
                    "Arkanoid");
}


World::~World()
{
}

void World::initialize()
{
    // set starting state
    this->changeState(new GameState(mWindow.get(), mTree));
}

void World::loop()
{
    this->initialize();
    
    while (mWindow->isOpen())
    {
        auto elapsed = mClock.restart().asSeconds();

        // update state
        mCurrentState->update(elapsed);
    }

    this->terminate();
}

void World::terminate()
{
    mCurrentState->exit();
}

void World::changeState(BaseState* newState)
{
    if (mCurrentState) {
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