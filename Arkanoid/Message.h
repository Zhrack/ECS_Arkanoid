#ifndef MESSAGE_H
#define MESSAGE_H

#include "pch.h"
#include "MessageTypes.h"

struct Message {
    unsigned long mSenderID;

    MessageType mType;

    void* mExtraInfo;

    Message() :
        mSenderID(0),
        mType(MessageType::MSG_NULL),
        mExtraInfo(nullptr)
    {
    }

    Message(unsigned long sender, MessageType type, void * extra = nullptr) :
        mSenderID(sender),
        mType(type),
        mExtraInfo(extra)
    {
    }

    virtual ~Message() {}
};

#endif // !MESSAGE_H
