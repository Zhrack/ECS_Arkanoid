#include "pch.h"
#include "GameState.h"

#include "PlayerInputComponent.h"

#include <iostream>

// start it from 1, 0 is reserved to be a NULL value
unsigned long GameState::nextID = 1;

GameState::GameState(sf::RenderWindow& window) :
    BaseState(window),
    mHighScore(0),
    mCurrentScore(0),
    mRemainingLives(Constants::NUM_LIVES),
    mCompMap(),
    mPlayerInputComp(nullptr)
{
}


GameState::~GameState()
{
    for (auto it = mCompMap.begin(); it != mCompMap.end(); ++it)
    {
        for (unsigned int i = 0; i < it->second.size(); i++)
        {
            delete it->second[i];
        }
        it->second.clear();
    }
}

void GameState::enter()
{
    std::cout << "GameState::enter" << std::endl;

    using CompPair = std::pair<CompType, std::vector<BaseComponent*>>;
    using UniquePtrVector = std::vector<BaseComponent*>;

    // initialize vectors
    mCompMap.insert({ 
        {CompType::PLAYER_INPUT, UniquePtrVector()},
    });

    auto entityID = this->createEntity();
    //auto entityID2 = this->createEntity();

    mPlayerInputComp = addComponent<PlayerInputComponent>(CompType::PLAYER_INPUT, entityID);
    //auto comp = addComponent<PlayerInputComponent>(CompType::PLAYER_INPUT, entityID2);
    
}

void GameState::update(int elapsed)
{
    elapsed;
    // handle inputs
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            mWindow.close();

        Message msg(0, MessageType::MSG_INPUT_EVENT, (void*)&event);
        mPlayerInputComp->receive(msg);

        mPlayerInputComp->update();
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

EntityID GameState::createEntity()
{
    return createID();
}

void GameState::removeComponent(CompType type, EntityID entityID)
{
    if (mCompMap.count(type) > 0)
    {
        auto& vec = mCompMap[type];

        auto res = std::find_if(vec.begin(), vec.end(),
            [entityID](BaseComponent* e) {

            return e->getEntityID() == entityID;
        });


        // swap with last element
        auto last = vec.back();
        vec[vec.size() - 1] = *res;
        vec[res - vec.begin()] = last;

        vec.pop_back();

    }
}

EntityID GameState::createID() const
{
    return this->nextID++;
}
