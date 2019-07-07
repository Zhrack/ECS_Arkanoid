
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
#include "GameOverWatcherComponent.h"

#include "World.h"
#include "MenuState.h"

#include "PCAudioService.h"
#include "ServiceLocator.h"
#include "PCTextureService.h"

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
    mGameStatus(GameStatus::GAME_NORMAL)
{
}


GameState::~GameState()
{
    cleanupZombies();
    for (auto it = mEntityMap.begin(); it != mEntityMap.end();)
    {
        EntityID i = it->first;
        ++it;
        removeEntity(i);
    }
}

void GameState::enter()
{
    std::cout << "GameState::enter" << std::endl;

    try
    {
        // register types
        using UniquePtrVector = std::vector<BaseComponent*>;
        for (size_t i = 0; i < CompType::COUNT; ++i)
        {
            mCompMap.insert({ CompType(i), UniquePtrVector() });
        }

        // load sounds
        auto audioService = new PCAudioService();

        std::string sfxFolder = mTree.get<std::string>("AUDIO_FOLDER");
        std::string musicFolder = mTree.get<std::string>("MUSIC_FOLDER");
        std::vector<SoundIDFilename> sounds(
            {
                {SoundID::BALL_HIT_BRICK, mTree.get<std::string>("AUDIO_BALL_HIT_BRICK")},
                {SoundID::BALL_HIT_WALL, mTree.get<std::string>("AUDIO_BALL_HIT_WALL")},
                {SoundID::POWER_UP_PICKED, mTree.get<std::string>("AUDIO_POWER_UP_PICKED")},
                {SoundID::BALL_LOST, mTree.get<std::string>("AUDIO_BALL_LOST")},
            });
        std::vector<MusicIDFilename> musics(
            {
                {MusicID::MUSIC_GAME, mTree.get<std::string>("GAME_MUSIC")},
                {MusicID::MUSIC_WIN, mTree.get<std::string>("WIN_MUSIC")},
                {MusicID::MUSIC_LOSE, mTree.get<std::string>("LOSE_MUSIC")},
            });

        if (!audioService->initialize(sfxFolder, musicFolder, sounds, musics))
        {
            throw std::exception("Error initializing the audio service");
        }

        ServiceLocator::provideAudioService(audioService);

        ServiceLocator::getAudioService()->playMusic(MusicID::MUSIC_GAME);
        ServiceLocator::getAudioService()->setLooping(MusicID::MUSIC_GAME, true);

        buildLevel();

        if (!mFont.loadFromFile("resources/joystix monospace.ttf"))
        {
            throw std::exception("Error loading font");
        }

        mRemainingLives = mTree.get<int>("NUM_LIVES");
        mHighScore = mTree.get<int>("HIGH_SCORE");

        mHighScoreText.setFont(mFont);
        mHighScoreNumberText.setFont(mFont);

        mCurrentScoreText.setFont(mFont);
        mCurrentScoreNumberText.setFont(mFont);

        mRemainingLivesText.setFont(mFont);
        mRemainingLivesNumberText.setFont(mFont);

        mGameOverText.setFont(mFont);
        mGameOverInstructionsText.setFont(mFont);
        mGameOverInstructions2Text.setFont(mFont);

        mHighScoreText.setString("High score");
        mHighScoreNumberText.setString(std::to_string(mHighScore));
        mCurrentScoreText.setString("Score");
        mCurrentScoreNumberText.setString(std::to_string(mCurrentScore));
        mRemainingLivesText.setString("Lives");
        mRemainingLivesNumberText.setString(std::to_string(mRemainingLives));
        mGameOverText.setString("GAME OVER!");
        mGameOverInstructionsText.setString("Enter to restart");
        mGameOverInstructions2Text.setString("Escape to return to Menu");

        sf::Vector2f textPos(
            mWalls.getPosition().x + mWalls.getSize().x + 10.f,
            mWalls.getPosition().y + 200.f);

        mHighScoreText.setPosition(textPos);

        textPos.y += mHighScoreText.getGlobalBounds().height + 5.f;
        mHighScoreNumberText.setPosition(textPos);

        textPos.y += mHighScoreNumberText.getGlobalBounds().height + 50.f;
        mCurrentScoreText.setPosition(textPos);

        textPos.y += mCurrentScoreText.getGlobalBounds().height + 5.f;
        mCurrentScoreNumberText.setPosition(textPos);

        textPos.y = mTree.get<float>("SCREEN_HEIGHT") - mRemainingLivesText.getGlobalBounds().height * 2.f - mRemainingLivesNumberText.getGlobalBounds().height;
        mRemainingLivesText.setPosition(textPos);

        textPos.y += mRemainingLivesNumberText.getGlobalBounds().height + 5.f;
        mRemainingLivesNumberText.setPosition(textPos);

        mGameOverText.setPosition(mWalls.getPosition().x + mWalls.getSize().x * 0.5f - mGameOverText.getGlobalBounds().width * 0.5f, mWalls.getPosition().y + 90.f);

        mGameOverInstructionsText.setPosition(mGameOverText.getPosition().x,
            mWalls.getSize().y * 0.5f + mGameOverText.getGlobalBounds().height + 50.f);

        mGameOverInstructions2Text.setPosition(mGameOverText.getPosition().x,
            mGameOverInstructionsText.getPosition().y + mGameOverInstructionsText.getGlobalBounds().height + mGameOverInstructions2Text.getGlobalBounds().height + 5.f);

        mHighScoreText.setCharacterSize(20);
        mHighScoreNumberText.setCharacterSize(20);
        mCurrentScoreText.setCharacterSize(22);
        mCurrentScoreNumberText.setCharacterSize(22);
        mRemainingLivesText.setCharacterSize(20);
        mRemainingLivesNumberText.setCharacterSize(20);

        mGameOverText.setCharacterSize(40);
        mGameOverInstructionsText.setCharacterSize(20);
        mGameOverInstructions2Text.setCharacterSize(20);

        mHighScoreText.setFillColor(sf::Color::Red);
        mCurrentScoreText.setFillColor(sf::Color::Red);
        mRemainingLivesText.setFillColor(sf::Color::Red);

        mGameOverText.setFillColor(sf::Color::Magenta);
        mGameOverInstructionsText.setFillColor(sf::Color::Magenta);
        mGameOverInstructions2Text.setFillColor(sf::Color::Magenta);

        auto textureService = new PCTextureService();

        std::string textureFolder = mTree.get<std::string>("TEXTURE_FOLDER");
        std::vector<TextureIDFilename> textures(
            {
                {TextureID::TEXTURE_BACKGROUND, mTree.get<std::string>("BACKGROUND"), sf::IntRect()},
                {TextureID::TEXTURE_LOGO, mTree.get<std::string>("LOGO_IMAGE"), sf::IntRect()},
                {TextureID::TEXTURE_BOMBER, mTree.get<std::string>("BOMBER_IMAGE"), sf::IntRect(0, 0, 109, 166)}
            });

        if (!textureService->initialize(textureFolder, textures))
        {
            throw std::exception("Error initializing the texture service");
        }

        ServiceLocator::provideTextureService(textureService);

        std::string filename = mTree.get<std::string>("BACKGROUND");

        sf::Texture* backgroundTex = ServiceLocator::getTextureService()->getTexture(TextureID::TEXTURE_BACKGROUND);
        mWalls.setTexture(backgroundTex);

        mPreviousTime = mClock.getElapsedTime().asSeconds();
        mTimeLag = 0.f;
        mMSPerUpdate = mTree.get<float>("MS_PER_UPDATE");
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
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
            if (mGameStatus == GameStatus::GAME_NORMAL)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                {
                    mPaddleBehaviorComp->onFireButtonPressed();
                }
            }
            else
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    // go to MENU
                    mWorld->changeState(new MenuState(mWorld, mTree));
                    return;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                {
                    restartGame();
                }
            }
        }
    }

    // update loop
    if (mGameStatus == GameStatus::GAME_NORMAL)
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
    auto disruptionList = getComponentList(CompType::DISRUPTION);
    auto bomberList = getComponentList(CompType::BOMBER);
    powerups.insert(powerups.end(), disruptionList.begin(), disruptionList.end());
    powerups.insert(powerups.end(), bomberList.begin(), bomberList.end());
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
    auto colliders = getComponentList(CompType::BOX_COLLIDER);
    auto circleColliders = getComponentList(CompType::CIRCLE_COLLIDER);
    colliders.insert(colliders.end(), circleColliders.begin(), circleColliders.end());

    mCollisionDetector.checkCollisions(colliders);

    auto watchers = getComponentList(CompType::GAME_OVER_WATCHER);
    for (auto e : watchers)
    {
        if (!e->isZombie())
        {
            e->updateComponent(elapsed);
        }
    }

    mHighScoreNumberText.setString(std::to_string(mHighScore));
    mCurrentScoreNumberText.setString(std::to_string(mCurrentScore));
    mRemainingLivesNumberText.setString(std::to_string(mRemainingLives));
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
    mWindow->draw(mHighScoreNumberText);
    mWindow->draw(mCurrentScoreText);
    mWindow->draw(mCurrentScoreNumberText);
    mWindow->draw(mRemainingLivesText);
    mWindow->draw(mRemainingLivesNumberText);

    if (mGameStatus != GameStatus::GAME_NORMAL)
    {
        mWindow->draw(mGameOverText);
        mWindow->draw(mGameOverInstructionsText);
        mWindow->draw(mGameOverInstructions2Text);
    }

    mWindow->display();
}

void GameState::buildLevel()
{
    sf::Vector2f screenSize(mTree.get<float>("SCREEN_WIDTH"), mTree.get<float>("SCREEN_HEIGHT"));

    float margin = 5.f;

    sf::Vector2f scalingFactor(0.8f, 1.0f);

    mWalls.setPosition(margin, margin);
    mWalls.setSize(sf::Vector2f(
        std::floorf(screenSize.x * scalingFactor.x), 
        std::floorf(screenSize.y * scalingFactor.y)));
    mWalls.setOutlineThickness(margin);
    mWalls.setOutlineColor(sf::Color::Red);


    sf::Vector2f paddleSize(mTree.get<float>("PADDLE_SIZE_X"), mTree.get<float>("PADDLE_SIZE_Y"));
    sf::Vector2f brickSize(mTree.get<float>("BRICK_SIZE_X"), mTree.get<float>("BRICK_SIZE_Y"));


    mPaddleID = this->createEntity(EntityType::TAG_PLAYER);

    addComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, mPaddleID, paddleSize);
    addComponent<RectRenderComponent>(CompType::RECT_RENDER, mPaddleID, paddleSize, sf::Color::Green);
    mPaddleBehaviorComp = addComponent<PaddleBehaviorComponent>(CompType::PADDLE_BEHAVIOR, mPaddleID,
        sf::Vector2f((float)mWalls.getSize().x * 0.5f - paddleSize.x * 0.5f, (float)(mWalls.getSize().y - paddleSize.y)));

    auto ballID = this->createEntity(EntityType::TAG_BALL);

    float ballRadius = mTree.get<float>("BALL_RADIUS");

    addComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, ballID, ballRadius);
    addComponent<CircleRenderComponent>(CompType::CIRCLE_RENDER, ballID, ballRadius, sf::Color::Red);
    mBallBehavior = addComponent<BallBehaviorComponent>(CompType::BALL_BEHAVIOR, ballID, mTree.get<float>("BALL_MAX_VELOCITY"), sf::Vector2f());
    mBallBehavior->lockBall();


    // create some bricks in a grid
    sf::Vector2f offset(0, 0);
    offset += sf::Vector2f(mWalls.getSize().x * 0.2f, mWalls.getSize().y * 0.2f);
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 7; j++)
        {
            auto brickID = this->createEntity(EntityType::TAG_BRICK);

            //sf::Vector2f pos = offset + sf::Vector2f(brickSize.x * i + (i * 13), brickSize.y * j + (j * 13));
            sf::Vector2f pos = offset + sf::Vector2f(brickSize.x * i, brickSize.y * j);

            BrickType type = BrickType::BRICK_NORMAL;

            if (j % 2 != 0) type = BrickType::BRICK_DOUBLE;
            if (j == 0) type = BrickType::BRICK_2HIT;

            addComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, brickID, brickSize);
            addComponent<RectRenderComponent>(CompType::RECT_RENDER, brickID, brickSize, sf::Color::Yellow, sf::Color::Black, 1.f);
            addComponent<BrickBehaviorComponent>(CompType::BRICK, brickID, pos, type);
        }
    }

    auto watcherID = createEntity(EntityType::TAG_GAME_OVER_WATCHER);
    addComponent<GameOverWatcherComponent>(CompType::GAME_OVER_WATCHER, watcherID);
}

void GameState::exit()
{
    std::cout << "GameState::exit" << std::endl;

    if (mTree.get<int>("HIGH_SCORE") < mHighScore)
    {
        mTree.put<int>("HIGH_SCORE", mHighScore);
        pt::write_json("resources/settings.json", mTree);
    }

    ServiceLocator::getAudioService()->stopMusic(MusicID::MUSIC_GAME);
    mWindow->clear();

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
}

void GameState::decrementPlayerLives()
{
    mRemainingLives--;

    if (mRemainingLives <= 0)
    {
        // :(
        gameOver(false);
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
        mBallBehavior->lockBall();
    }
}

int GameState::getPlayerLives() const
{
    return mRemainingLives;
}

void GameState::gameOver(bool win)
{
    if (win)
    {
        mGameStatus = GameStatus::GAME_WIN;
        mGameOverText.setString("You WIN!");
        ServiceLocator::getAudioService()->stopMusic(MusicID::MUSIC_GAME);
        ServiceLocator::getAudioService()->playMusic(MusicID::MUSIC_WIN);
    }
    else
    {
        mGameStatus = GameStatus::GAME_LOSE;
        mGameOverText.setString("GAME OVER!");
        ServiceLocator::getAudioService()->stopMusic(MusicID::MUSIC_GAME);
        ServiceLocator::getAudioService()->playMusic(MusicID::MUSIC_LOSE);
    }
}

void GameState::restartGame()
{
    cleanupZombies();

    for (auto it = mEntityMap.begin(); it != mEntityMap.end();)
    {
        EntityID i = it->first;
        ++it;
        removeEntity(i);
    }

    mGameStatus = GameStatus::GAME_NORMAL;
    mRemainingLives = mTree.get<int>("NUM_LIVES");
    mCurrentScore = 0;

    ServiceLocator::getAudioService()->stopMusic(MusicID::MUSIC_GAME);
    ServiceLocator::getAudioService()->playMusic(MusicID::MUSIC_GAME);
    ServiceLocator::getAudioService()->setLooping(MusicID::MUSIC_GAME, true);

    buildLevel();

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
