#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "BaseState.h"

#include "ComponentList.h"
#include "EntityTypes.h"
#include "BaseComponent.h"
#include "CollisionDetector.h"
#include "PowerUpService.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using ComponentMap = 
std::unordered_map<
    CompType,
    std::vector< BaseComponent* >
>;

class PaddleBehaviorComponent;
class RectRenderComponent;
class BallBehaviorComponent;

namespace pt = boost::property_tree;

/// <summary>
/// State that manages the actual game
/// </summary>
/// <seealso cref="BaseState" />
/// <seealso cref="std::enable_shared_from_this{GameState}" />
class GameState :
    public BaseState
{
public:
    GameState(World* world, pt::ptree& tree);
    virtual ~GameState();

    // Inherited via BaseState
    virtual void enter() override;
    virtual void update() override;
    virtual void exit() override;

    EntityID createEntity(EntityType type);

    /// <summary>
    /// Variadic template funciton that adds a new component.
    /// </summary>
    /// <param name="type">The type ID of the component.</param>
    /// <param name="entityID">The entity identifier.</param>
    /// <returns></returns>
    template<class T, class... Args>
    inline T* addComponent(CompType type, EntityID entityID, Args... args);

    std::vector<BaseComponent*>& getComponentList(CompType type);

    pt::ptree& config();

    /// <summary>
    /// Gets the requested component. The template parameter is used to downcast directly to the right derived type.
    /// </summary>
    /// <param name="type">The component enum.</param>
    /// <param name="entityID">The entity identifier.</param>
    /// <returns></returns>
    template<class T>
    T* getComponent(CompType type, EntityID entityID);

    void destroyEntity(EntityID entityID);

    void removeComponent(CompType type, EntityID entityID);

    EntityType getEntityType(EntityID entityID);

    std::vector<EntityID> getAllEntitiesByType(EntityType type);

    void increaseScore(long points);

    PaddleBehaviorComponent* getPaddleComponent();

    void sendMessage(EntityType type, CompType compType, Message & msg, SendType sendType = SendType::ENQUEUE);
    void sendMessage(CompType compType, EntityID entityID, Message& msg, SendType sendType = SendType::ENQUEUE);

    PowerUpService& getPUService();

private:
    void removeEntity(EntityID entityID);

    /// <summary>
    /// What a cool name!
    /// </summary>
    void cleanupZombies();

    void updateGame(float elapsed);

    void renderGame(float elapsed);

private:

    /// <summary>
    /// General clock for the game.
    /// </summary>
    sf::Clock mClock;

    float mCurrentTime;
    float mPreviousTime;
    float mTimeLag;
    float mMSPerUpdate;

    // general game data
    long mHighScore;
    long mCurrentScore;
    int mRemainingLives;

    EntityID mPaddleID;

    /// <summary>
    /// The used to generate IDs.
    /// </summary>
    static EntityID nextID;

    EntityID createID() const;

    /// <summary>
    /// Contains all components organized by type.
    /// </summary>
    ComponentMap mCompMap;

    /// <summary>
    /// To map every entity to a specific game type
    /// </summary>
    std::unordered_map<EntityID, EntityType> mEntityMap;

    std::vector<EntityID> mZombieEntities;

    PaddleBehaviorComponent* mPaddleBehaviorComp;

    BallBehaviorComponent* mBallBehavior;

    CollisionDetector mCollisionDetector;

    pt::ptree& mTree;

    PowerUpService mPUService;
};


#endif // !GAME_STATE_H

template<class T, class... Args>
inline T* GameState::addComponent(CompType type, EntityID entityID, Args... args)
{
    static_assert(std::is_base_of<BaseComponent, T>::value, "T must derive from BaseComponent");

    if (mCompMap.count(type) > 0)
    {
        auto ptr = new T(entityID, this, args...);
        mCompMap[type].push_back(ptr);
        return ptr;
    }

    return nullptr;
}

template<class T>
inline T * GameState::getComponent(CompType type, EntityID entityID)
{
    static_assert(std::is_base_of<BaseComponent, T>::value, "T must derive from BaseComponent");

    if (mCompMap.count(type) > 0)
    {
        auto& vec = mCompMap[type];

        auto res = std::find_if(vec.begin(), vec.end(), 
            [entityID](BaseComponent* e) {

            return e->getEntityID() == entityID;
        });

        return res == vec.end() ? nullptr : dynamic_cast<T*>(*res);
    }

    return nullptr;
}
