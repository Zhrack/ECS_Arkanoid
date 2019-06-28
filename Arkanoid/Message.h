#ifndef MESSAGE_H
#define MESSAGE_H

#include "pch.h"

enum MessageType {
    MSG_NULL = 0,

};

struct Message {
    unsigned long mSenderID;

    unsigned long mReceiverID;

    MessageType mType;

    void* mExtraInfo;

    Message(unsigned long sender, unsigned long receiver, MessageType type, void* extra = nullptr) :
        mSenderID(sender),
        mReceiverID(receiver),
        mType(type),
        mExtraInfo(extra) 
    {
    }

    virtual ~Message() {}
};

#endif // !MESSAGE_H
