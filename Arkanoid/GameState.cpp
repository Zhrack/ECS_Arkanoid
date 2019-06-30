#include "pch.h"
#include "GameState.h"

#include "PlayerInputComponent.h"
#include "TransformComponent.h"
#include "RectRenderComponent.h"
#include "BoxColliderComponent.h"
#include "BallBehaviorComponent.h"

#include <iostream>

// start it from 1, 0 is reserved to be a NULL value
unsigned long GameState::nextID = 1;

GameState::GameState(sf::RenderWindow* window) :
    BaseState(window),
    mHighScore(0),
    mCurrentScore(0),
    mRemainingLives(Constants::NUM_LIVES),
    mCompMap(),
    mPlayerInputComp(nullptr)
{
    using UniquePtrVector = std::vector<BaseComponent*>;

    // register types
    for (size_t i = 0; i < CompType::COUNT; ++i)
    {
        mCompMap.insert({ CompType(i), UniquePtrVector()});
    }

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

    auto entityID = this->createEntity();
    //auto entityID2 = this->createEntity();


    addComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, entityID, sf::Vector2f(Constants::PADDLE_SIZE_X, Constants::PADDLE_SIZE_Y));
    mPlayerInputComp = addComponent<PlayerInputComponent>(CompType::PLAYER_INPUT, entityID);
    addComponent<RectRenderComponent>(CompType::RENDER, entityID, sf::Vector2f(Constants::PADDLE_SIZE_X, Constants::PADDLE_SIZE_Y), sf::Color::Green);

    auto ballID = this->createEntity();

    addComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, ballID, sf::Vector2f(Constants::PADDLE_SIZE_X, Constants::PADDLE_SIZE_Y));
    mBallBehavior = addComponent<BallBehaviorComponent>(CompType::BALL, ballID, sf::Vector2f(200.0f, 200.0f));
    addComponent<RectRenderComponent>(CompType::RENDER, ballID, sf::Vector2f(Constants::PADDLE_SIZE_X, Constants::PADDLE_SIZE_Y), sf::Color::Red);

    // create some bricks in a grid
    //for (size_t i = 0; i < 20; i++)
    //{

    //}
}

void GameState::update(float elapsed)
{
    // update
    mPlayerInputComp->update(elapsed);
    // late update for collision detection and other "physics" stuff
    mBallBehavior->update(elapsed);
    // render step
    mWindow->clear();

    // call all renderComponents
    auto renderVector = getComponentList(CompType::RENDER);
    for (auto e : renderVector)
    {
        e->update(elapsed);
    }

    mWindow->display();
}

void GameState::exit()
{
    std::cout << "GameState::exit" << std::endl;
}

EntityID GameState::createEntity()
{
    auto id = createID();
    
    // TransformComponent is always added
    addComponent<TransformComponent>(CompType::TRANSFORM, id);

    return id;
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

void GameState::removeEntity(EntityID entityID)
{
    for (size_t i = 0; i < CompType::COUNT; i++)
    {
        removeComponent(CompType(i), entityID);
    }
}

std::vector<BaseComponent*>& GameState::getComponentList(CompType type)
{
    //static_assert(std::is_base_of<BaseComponent, T>::value, "T must derive from BaseComponent");

    return mCompMap[type];
}

EntityID GameState::createID() const
{
    return this->nextID++;
}
