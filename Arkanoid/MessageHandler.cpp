#include "pch.h"
#include "MessageHandler.h"


MessageHandler::MessageHandler() : 
    mQueue()
{
}


MessageHandler::~MessageHandler()
{
}

void MessageHandler::receive(Message&& msg, SendType sendType)
{
    switch (sendType)
    {
    case IMMEDIATE:
        this->handleMessage(msg);
        break;
    case ENQUEUE:
    default:
        mQueue.push(std::move(msg));
        break;
    }
}

Message MessageHandler::getNext()
{
    if (this->isEmpty())
    {
        return Message(0, 0, MessageType::MSG_NULL);
    }

    auto msg = mQueue.front();
    mQueue.pop();

    return msg;
}

bool MessageHandler::isEmpty() const
{
    return mQueue.empty();
}
