#ifndef MESSAGE_H
#define MESSAGE_H


#include "MessageTypes.h"

#include <SFML/System.hpp>
#include <memory>

struct Message {
    unsigned long mSenderID;

    MessageType mType;

    std::shared_ptr<BaseData> mExtraInfo;

    sf::Time mSendingTime;

    Message() :
        mSenderID(0),
        mType(MessageType::MSG_NULL),
        mExtraInfo(nullptr),
        mSendingTime()
    {
    }

    Message(unsigned long sender, MessageType type, std::shared_ptr<BaseData> extra = nullptr) :
        mSenderID(sender),
        mType(type),
        mExtraInfo(std::move(extra))
    {
    }

    ~Message() {}
};

#endif // !MESSAGE_H
