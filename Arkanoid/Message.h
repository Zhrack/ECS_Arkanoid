#ifndef MESSAGE_H
#define MESSAGE_H


#include "MessageTypes.h"

#include <SFML/System.hpp>

struct Message {
    unsigned long mSenderID;

    MessageType mType;

    void* mExtraInfo;

    sf::Time mSendingTime;

    Message() :
        mSenderID(0),
        mType(MessageType::MSG_NULL),
        mExtraInfo(nullptr),
        mSendingTime()
    {
    }

    Message(unsigned long sender, MessageType type, void * extra = nullptr) :
        mSenderID(sender),
        mType(type),
        mExtraInfo(extra)
    {
    }

    ~Message() {}
};

#endif // !MESSAGE_H
