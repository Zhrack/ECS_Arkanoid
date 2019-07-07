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
    MSG_LOCK_BALL,

    // POWERUP COLLISIONS
    MSG_PU_END_EFFECT,
    MSG_PU_STICKY,
    MSG_PU_DISRUPTION,
    MSG_PU_BOMBER,

    MSG_BALL_LOST,
    MSG_BRICK_DESTROYED,

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
/// For MSG_PU_STICKY messages
/// </summary>
struct StickyData : BaseData
{
    bool start;

    virtual ~StickyData() {}
};

/// <summary>
/// For MSG_PU_BOMBER messages
/// </summary>
struct BomberData : BaseData
{
    bool start;

    virtual ~BomberData() {}
};

#endif // !MESSAGE_TYPES_H

