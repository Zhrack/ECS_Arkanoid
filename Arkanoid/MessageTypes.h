#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

#include "ComponentList.h"

#include <SFML/Graphics.hpp>

class BaseComponent;

/// <summary>
/// Enum with all message types
/// </summary>
enum MessageType {
    MSG_NULL = 0,
    MSG_INPUT_EVENT, // test only
    MSG_MOVE,
    MSG_COLLISION,
    MSG_RELEASE_BALL,

    // POWERUP COLLISIONS
    MSG_PU_END_EFFECT,
    MSG_PU_STICKY,

};

// Following, structs to be sent with specific message types are defined here.

struct BaseData {
    virtual ~BaseData() {}
};

/// <summary>
/// For MSG_COLLISION messages
/// </summary>
struct CollisionData : BaseData
{
    BaseComponent* otherCollider;
    CompType otherType;
    sf::Vector2f amount;

    virtual ~CollisionData() {}
};

/// <summary>
/// For MSG_COLLISION messages
/// </summary>
struct StickyData : BaseData
{
    bool start;

    virtual ~StickyData() {}
};

#endif // !MESSAGE_TYPES_H

