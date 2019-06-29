#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "BaseState.h"

#include "ComponentList.h"
#include "BaseComponent.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

using ComponentMap = 
std::unordered_map<
    CompType,
    std::vector< BaseComponent* >
>;

class PlayerInputComponent;

/// <summary>
/// State that manages the actual game
/// </summary>
/// <seealso cref="BaseState" />
/// <seealso cref="std::enable_shared_from_this{GameState}" />
class GameState :
    public BaseState,
    public std::enable_shared_from_this<GameState>
{
public:
    GameState(sf::RenderWindow& window);
    virtual ~GameState();

    // Inherited via BaseState
    virtual void enter() override;
    virtual void update(int elapsed) override;
    virtual void exit() override;

    //TODO: createEntity, addComponent, getComponent, removeComponent
    EntityID createEntity();

    /// <summary>
    /// Template funciton that adds a new component.
    /// </summary>
    /// <param name="type">The type ID of the component.</param>
    /// <param name="entityID">The entity identifier.</param>
    /// <returns></returns>
    template<class T>
    T* addComponent(CompType type, EntityID entityID);

    template<class T>
    std::vector<T>& getComponentList(CompType type);

    template<class T>
    T* getComponent(CompType type, EntityID entityID);

    void removeComponent(CompType type, EntityID entityID);

private:
    // general game data
    long mHighScore;
    long mCurrentScore;
    int mRemainingLives;

    /// <summary>
    /// The used to generate IDs.
    /// </summary>
    static EntityID nextID;

    EntityID createID() const;

    /// <summary>
    /// Contains all components organized by type.
    /// </summary>
    ComponentMap mCompMap;

    PlayerInputComponent* mPlayerInputComp;
};


#endif // !GAME_STATE_H

template<class T>
inline T* GameState::addComponent(CompType type, EntityID entityID)
{
    static_assert(std::is_base_of<BaseComponent, T>::value, "T must derive from BaseComponent");

    if (mCompMap.count(type) > 0)
    {
        auto ptr = new T(entityID);
        mCompMap[type].push_back(ptr);
        return ptr;
    }

    return nullptr;
}

template<class T>
inline std::vector<T>& GameState::getComponentList(CompType type)
{
    static_assert(std::is_base_of<BaseComponent, T>::value, "T must derive from BaseComponent");

    if (mCompMap.count(type) > 0)
    {
        return mCompMap[type];
    }

    return std::vector<T>(0);
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
