
#include "GameState.h"

#include "PaddleBehaviorComponent.h"
#include "TransformComponent.h"
#include "RectRenderComponent.h"
#include "CircleRenderComponent.h"
#include "SpriteRenderComponent.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "BallBehaviorComponent.h"
#include "BrickBehaviorComponent.h"
#include "GameOverWatcher.h"

#include <iostream>
#include <thread>

// start it from 1, 0 is reserved to be a NULL value
unsigned long GameState::nextID = 1;

GameState::GameState(World* world, pt::ptree& tree) :
    BaseState(world),
    mHighScore(0),
    mCurrentScore(0),
    mRemainingLives(),
    mCompMap(),
    mEntityMap(),
    mZombieEntities(),
    mPaddleBehaviorComp(nullptr),
    mTree(tree),
    mPUService(this),
    mGameOver(false)
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
    cleanupZombies();
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

    buildLevel();

    auto watcherID = createEntity(EntityType::TAG_GAME_OVER_WATCHER);
    addComponent<GameOverWatcher>(CompType::GAME_OVER_WATCHER, watcherID);

    if (!mFont.loadFromFile("sprites/arcadeclassic.regular.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
    }

    mRemainingLives = mTree.get<int>("NUM_LIVES");
    mHighScore = mTree.get<int>("HIGH_SCORE");

    mHighScoreText.setFont(mFont);
    mCurrentScoreText.setFont(mFont);
    mRemainingLivesText.setFont(mFont);

    mHighScoreText.setString("High score " + std::to_string(mHighScore));
    mCurrentScoreText.setString("Score " + std::to_string(mCurrentScore));
    mRemainingLivesText.setString("Remaining lives " + std::to_string(mRemainingLives));

    sf::Vector2f textPos(
        mWalls.getPosition().x + mWalls.getSize().x + 10.f,
        mWalls.getPosition().y + 200.f);

    mHighScoreText.setPosition(textPos);
    mCurrentScoreText.setPosition(textPos.x, textPos.y + mHighScoreText.getGlobalBounds().height + 10.f);

    mRemainingLivesText.setPosition(
        mCurrentScoreText.getPosition().x,
        mCurrentScoreText.getPosition().y + mCurrentScoreText.getGlobalBounds().height + 10.f);

    mHighScoreText.setCharacterSize(24);
    mCurrentScoreText.setCharacterSize(24);
    mRemainingLivesText.setCharacterSize(24);

    mHighScoreText.setFillColor(sf::Color::Red);
    mCurrentScoreText.setFillColor(sf::Color::Red);
    mRemainingLivesText.setFillColor(sf::Color::Red);
        
    mPreviousTime = mClock.getElapsedTime().asSeconds();
    mTimeLag = 0.f;
    mMSPerUpdate = mTree.get<float>("MS_PER_UPDATE");
}

void GameState::update()
{
    mCurrentTime = mClock.getElapsedTime().asSeconds();
    float elapsed = mCurrentTime - mPreviousTime;
    mPreviousTime = mCurrentTime;
    mTimeLag += elapsed;


    sf::Event event;
    while (mWindow->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            mWindow->close();
            break;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                mWindow->close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                mPaddleBehaviorComp->onFireButtonPressed();
            }
        }
    }

    // update loop
    if (!mGameOver)
    {
        while (mTimeLag >= mMSPerUpdate)
        {
            updateGame(mMSPerUpdate);
            mTimeLag -= mMSPerUpdate;
        }
    }
    // render step
    renderGame(mTimeLag / mMSPerUpdate);

    // do some cleanup
    cleanupZombies();
}


void GameState::updateGame(float elapsed)
{
    // update
    mPaddleBehaviorComp->updateComponent(elapsed);
    auto balls = getComponentList(CompType::BALL_BEHAVIOR);
    for (auto e : balls)
    {
        if (!e->isZombie())
        {
            e->updateComponent(elapsed);
        }
    }
    auto powerups = getComponentList(CompType::STICKY);
    // append other power ups here...
    for (auto e : powerups)
    {
        if (!e->isZombie())
        {
            e->updateComponent(elapsed);
        }
    }
    // late update for collision detection and other "physics" stuff

    // make copies of colliders
    auto boxColliders = getComponentList(CompType::BOX_COLLIDER);
    auto circleColliders = getComponentList(CompType::CIRCLE_COLLIDER);
    boxColliders.insert(boxColliders.end(), circleColliders.begin(), circleColliders.end());

    mCollisionDetector.checkCollisions(boxColliders);

    auto watchers = getComponentList(CompType::GAME_OVER_WATCHER);
    for (auto e : watchers)
    {
        if (!e->isZombie())
        {
            e->updateComponent(elapsed);
        }
    }

    mHighScoreText.setString("High score " + std::to_string(mHighScore));
    mCurrentScoreText.setString("Score " + std::to_string(mCurrentScore));
    mRemainingLivesText.setString("Remaining lives " + std::to_string(mRemainingLives));
}

void GameState::renderGame(float elapsed)
{
    mWindow->clear();

    mWindow->draw(mWalls);

    // call all renderComponents
    auto renderVector = getComponentList(CompType::RECT_RENDER);
    auto circleRenderVector = getComponentList(CompType::CIRCLE_RENDER);
    auto spriteRenderVector = getComponentList(CompType::SPRITE_RENDER);


    // here if needed, sorting of rendererComponents could be made
    renderVector.insert(renderVector.end(), circleRenderVector.begin(), circleRenderVector.end());
    renderVector.insert(renderVector.end(), spriteRenderVector.begin(), spriteRenderVector.end());

    for (auto e : renderVector)
    {
        if (!e->isZombie())
        {
            e->updateComponent(elapsed);
        }
    }

    mWindow->draw(mHighScoreText);
    mWindow->draw(mCurrentScoreText);
    mWindow->draw(mRemainingLivesText);

    mWindow->display();
}

void GameState::buildLevel()
{
    sf::Vector2f screenSize(mTree.get<float>("SCREEN_WIDTH"), mTree.get<float>("SCREEN_HEIGHT"));

    //sf::Vector2f wallSize(screenSize.x - 100.f, screenSize.y - 10.f);

    float margin = 5.f;

    sf::Vector2f scalingFactor(0.8f, 1.0f);

    mWalls.setPosition(margin, margin);
    mWalls.setSize(sf::Vector2f(
        std::floorf(screenSize.x * scalingFactor.x), 
        std::floorf(screenSize.y * scalingFactor.y)));
    mWalls.setFillColor(sf::Color::Transparent);
    mWalls.setOutlineThickness(margin);
    mWalls.setOutlineColor(sf::Color::Red);


    sf::Vector2f paddleSize(mTree.get<float>("PADDLE_SIZE_X"), mTree.get<float>("PADDLE_SIZE_Y"));
    sf::Vector2f brickSize(mTree.get<float>("BRICK_SIZE_X"), mTree.get<float>("BRICK_SIZE_Y"));


    mPaddleID = this->createEntity(EntityType::TAG_PLAYER);

    addComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, mPaddleID, paddleSize);
    addComponent<RectRenderComponent>(CompType::RECT_RENDER, mPaddleID, paddleSize, sf::Color::Green);
    mPaddleBehaviorComp = addComponent<PaddleBehaviorComponent>(CompType::PADDLE_BEHAVIOR, mPaddleID,
        sf::Vector2f((float)mWalls.getSize().x / 2.f, (float)(mWalls.getSize().y - paddleSize.y)));

    auto ballID = this->createEntity(EntityType::TAG_BALL);

    float ballRadius = mTree.get<float>("BALL_RADIUS");

    addComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, ballID, ballRadius);
    addComponent<CircleRenderComponent>(CompType::CIRCLE_RENDER, ballID, ballRadius, sf::Color::Red);
    mBallBehavior = addComponent<BallBehaviorComponent>(CompType::BALL_BEHAVIOR, ballID, mTree.get<float>("BALL_MAX_VELOCITY"), sf::Vector2f());
    mBallBehavior->changeState(BallState::BALL_FOLLOW_PADDLE);


    // create some bricks in a grid
    sf::Vector2f offset(mWalls.getPosition());
    offset += sf::Vector2f(mWalls.getSize().x * 0.2f, mWalls.getSize().y * 0.2f);
    for (size_t i = 0; i < 6; i++)
    {
        for (size_t j = 0; j < 5; j++)
        {
            auto brickID = this->createEntity(EntityType::TAG_BRICK);

            addComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, brickID, brickSize);
            addComponent<RectRenderComponent>(CompType::RECT_RENDER, brickID, brickSize, sf::Color::Blue);
            addComponent<BrickBehaviorComponent>(CompType::BRICK, brickID, sf::Vector2f(brickSize.x * i + (i*13), brickSize.y * j + (j*13)) + offset);
        }
    }
}

void GameState::exit()
{
    std::cout << "GameState::exit" << std::endl;
    mTree.put<int>("HIGH_SCORE", mHighScore);

    pt::write_json("settings.json", mTree);
}

EntityID GameState::createEntity(EntityType type)
{
    auto id = createID();
    
    // TransformComponent is always added
    addComponent<TransformComponent>(CompType::TRANSFORM, id);

    mEntityMap[id] = type;

    return id;
}

void GameState::destroyEntity(EntityID entityID)
{
    // search if it has been already marked for removal
    auto res = std::find_if(mZombieEntities.begin(), mZombieEntities.end(),
        [entityID](EntityID e) {

        return e == entityID;
    });

    if (res != mZombieEntities.end())
    {
        return;
    }
    // tag all its components as zombies
    for (auto it = mCompMap.begin(); it != mCompMap.end(); ++it)
    {
        for (unsigned int i = 0; i < it->second.size(); i++)
        {
            if (it->second[i]->getEntityID() == entityID)
            {
                it->second[i]->setZombie();
            }
        }
    }

    mZombieEntities.push_back(entityID);
}

void GameState::removeComponent(CompType type, EntityID entityID)
{
    if (mCompMap.count(type) > 0)
    {
        auto& vec = mCompMap[type];

        size_t i = 0;
        for (; i < vec.size(); i++)
        {
            if (vec[i]->getEntityID() == entityID)
            {
                break;
            }
        }

        if (i == vec.size()) return;


        // swap with last element
        std::iter_swap(vec.begin() + i, vec.end() - 1);
        delete *(vec.end() - 1);
        vec.pop_back();
    }
}

void GameState::removeEntity(EntityID entityID)
{
    for (size_t i = 0; i < CompType::COUNT; i++)
    {
        removeComponent(CompType(i), entityID);
    }

    mEntityMap.erase(entityID);
}

void GameState::cleanupZombies()
{
    for (auto id : mZombieEntities)
    {
        removeEntity(id);
    }

    mZombieEntities.clear();
}

EntityType GameState::getEntityType(EntityID entityID)
{
    if (mEntityMap.count(entityID) > 0)
    {
        return mEntityMap[entityID];
    }
    return EntityType::TAG_NONE;
}

std::vector<EntityID> GameState::getAllEntitiesByType(EntityType type)
{
    std::vector<EntityID> result;

    std::for_each(mEntityMap.begin(), mEntityMap.end(), [&](std::pair<EntityID, EntityType> pair)
    {
        if (pair.second == type)
        {
            result.push_back(pair.first);
        }
    });

    return result;
}

void GameState::increaseScore(long points)
{
    mCurrentScore += points;

    if (mCurrentScore > mHighScore)
    {
        mHighScore = mCurrentScore;
    }

    std::cout << "SCORE: " << mCurrentScore << std::endl;
}

void GameState::decrementPlayerLives()
{
    mRemainingLives--;

    if (mRemainingLives == 0)
    {
        // :(
        gameOver();
    }
    else
    {
        // spawn a new ball
        auto ballID = this->createEntity(EntityType::TAG_BALL);

        float ballRadius = mTree.get<float>("BALL_RADIUS");

        addComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, ballID, ballRadius);
        addComponent<CircleRenderComponent>(CompType::CIRCLE_RENDER, ballID, ballRadius, sf::Color::Red);
        mBallBehavior = addComponent<BallBehaviorComponent>(CompType::BALL_BEHAVIOR, ballID, mTree.get<float>("BALL_MAX_VELOCITY"), sf::Vector2f());
        mBallBehavior->changeState(BallState::BALL_FOLLOW_PADDLE);

        mPaddleBehaviorComp->changeState(PaddleState::STATE_START);
    }
}

int GameState::getPlayerLives() const
{
    return mRemainingLives;
}

void GameState::gameOver()
{
    mGameOver = true;
}

PaddleBehaviorComponent * GameState::getPaddleComponent()
{
    return getComponent<PaddleBehaviorComponent>(CompType::PADDLE_BEHAVIOR, mPaddleID);
}

void GameState::sendMessage(EntityType type, CompType compType, Message & msg, SendType sendType, const sf::Time& timeToFire)
{
    // small optimization for often accessed component
    if (type == TAG_PLAYER)
    {
        getPaddleComponent()->receive(msg, sendType, timeToFire);
        return;
    }

    for (auto e : mEntityMap)
    {
        if (e.second == type)
        {
            getComponent<BaseComponent>(compType, e.first)->receive(msg, sendType, timeToFire);
        }
    }
}

void GameState::sendMessage(CompType compType, EntityID entityID, Message & msg, SendType sendType, const sf::Time& timeToFire)
{
    getComponent<BaseComponent>(compType, entityID)->receive(msg, sendType, timeToFire);
}

PowerUpService & GameState::getPUService()
{
    return mPUService;
}

std::vector<BaseComponent*>& GameState::getComponentList(CompType type)
{
    // no check needed, all CompTypes auto register
    return mCompMap[type];
}

pt::ptree & GameState::config()
{
    return mTree;
}

const sf::RectangleShape& GameState::getWalls() const
{
    return mWalls;
}

EntityID GameState::createID() const
{
    return this->nextID++;
}
