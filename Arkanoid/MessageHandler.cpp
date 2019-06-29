#include "pch.h"
#include "MessageHandler.h"


MessageHandler::MessageHandler() : 
    mQueue()
{
}


MessageHandler::~MessageHandler()
{
}

void MessageHandler::receive(Message& msg, SendType sendType)
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

bool MessageHandler::getNext(Message& msg)
{
    if (this->isEmpty())
    {
        return false;
    }

    msg = mQueue.front();
    mQueue.pop();

    return true;
}

void MessageHandler::pullMessages()
{
    Message msg;
    while (this->getNext(msg))
    {
        this->handleMessage(msg);
    }
}

bool MessageHandler::isEmpty() const
{
    return mQueue.empty();
}
