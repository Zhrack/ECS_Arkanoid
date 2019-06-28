#include "pch.h"
#include "GameState.h"

#include <iostream>

GameState::GameState(sf::RenderWindow& window) :
    BaseState(window),
    mHighScore(0),
    mCurrentScore(0),
    mRemainingLives(Constants::NUM_LIVES)
{
}


GameState::~GameState()
{
}

void GameState::enter()
{
    std::cout << "GameState::enter" << std::endl;
}

void GameState::update(int elapsed)
{
    std::cout << "GameState::update " << elapsed << std::endl;

    // handle inputs
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            mWindow.close();
    }

    // update

    // late update for collision detection and other "physics" stuff

    // render step
    mWindow.clear();
    mWindow.display();
}

void GameState::exit()
{
    std::cout << "GameState::exit" << std::endl;
}
