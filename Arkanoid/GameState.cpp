#include "pch.h"
#include "GameState.h"

#include "PlayerInputComponent.h"
#include "TransformComponent.h"
#include "RectRenderComponent.h"
#include "CircleRenderComponent.h"
#include "SpriteRenderComponent.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "BallBehaviorComponent.h"

#include <iostream>

// start it from 1, 0 is reserved to be a NULL value
unsigned long GameState::nextID = 1;

GameState::GameState(sf::RenderWindow* window, pt::ptree& tree) :
    BaseState(window),
    mHighScore(0),
    mCurrentScore(0),
    mRemainingLives(),
    mCompMap(),
    mPlayerInputComp(nullptr),
    mTree(tree)
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

    mRemainingLives = mTree.get<int>("NUM_LIVES");

    auto entityID = this->createEntity();
    //auto entityID2 = this->createEntity();

    sf::Vector2f paddleSize(mTree.get<float>("PADDLE_SIZE_X"), mTree.get<float>("PADDLE_SIZE_Y"));


    addComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, entityID, paddleSize);
    mPlayerInputComp = addComponent<PlayerInputComponent>(CompType::PLAYER_INPUT, entityID);
    addComponent<RectRenderComponent>(CompType::RECT_RENDER, entityID, paddleSize, sf::Color::Green);

    auto ballID = this->createEntity();

    addComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, ballID, paddleSize.x);
    mBallBehavior = addComponent<BallBehaviorComponent>(CompType::BALL, ballID, sf::Vector2f(200.0f, 200.0f));
    addComponent<CircleRenderComponent>(CompType::CIRCLE_RENDER, ballID, 50.f, sf::Color::Red);

    // create some bricks in a grid
    //for (size_t i = 0; i < 20; i++)
    //{

    //}
}

void GameState::update(float elapsed)
{
    // update
    mPlayerInputComp->update(elapsed);
    mBallBehavior->update(elapsed);
    // late update for collision detection and other "physics" stuff

    // make copies of colliders
    auto boxColliders = getComponentList(CompType::BOX_COLLIDER);
    auto circleColliders = getComponentList(CompType::CIRCLE_COLLIDER);
    boxColliders.insert(boxColliders.end(), circleColliders.begin(), circleColliders.end());

    mCollisionDetector.checkCollisions(boxColliders);
    // render step
    mWindow->clear();

    // call all renderComponents
    auto renderVector = getComponentList(CompType::RECT_RENDER);
    auto circleRenderVector = getComponentList(CompType::CIRCLE_RENDER);
    auto spriteRenderVector = getComponentList(CompType::SPRITE_RENDER);


    renderVector.insert(renderVector.end(), circleRenderVector.begin(), circleRenderVector.end());
    renderVector.insert(renderVector.end(), spriteRenderVector.begin(), spriteRenderVector.end());

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

pt::ptree & GameState::config()
{
    return mTree;
}

EntityID GameState::createID() const
{
    return this->nextID++;
}
