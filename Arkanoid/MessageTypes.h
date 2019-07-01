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
    MSG_COLLISION
};

// Following, structs to be sent with specific message types are defined here.

struct CollisionData
{
    BaseComponent* other;
    CompType otherType;
    sf::Vector2f amount;
};

#endif // !MESSAGE_TYPES_H

