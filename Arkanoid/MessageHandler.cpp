
#include "MessageHandler.h"


MessageHandler::MessageHandler() : 
    mQueue(),

    mDelayedMessages()
{
    mClock.restart();
}


MessageHandler::~MessageHandler()
{
}

void MessageHandler::receive(Message& msg, SendType sendType, const sf::Time& timeToFire)
{
    switch (sendType)
    {
    case DELAYED:
        mDelayedMessages.push_back(std::make_pair(mClock.getElapsedTime() + timeToFire, msg));
        // keep sorted by order of firing
        mDelayedMessages.sort([](const DelayedMsg& rhs, const DelayedMsg& lhs) {
            return rhs.first > lhs.first;
        });
        break;
    case IMMEDIATE:
        this->handleMessage(msg);
        break;
    case ENQUEUE:
    default:
        mQueue.push_back(std::move(msg));
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

    mQueue.pop_front();

    return true;
}

void MessageHandler::pullMessages()
{
    getReadyMessages();

    Message msg;
    while (this->getNext(msg))
    {
        this->handleMessage(msg);
    }
}

void MessageHandler::getReadyMessages()
{
    if (mDelayedMessages.empty()) return;

    bool found = false;

    auto it = mDelayedMessages.begin();
    while (it != mDelayedMessages.end())
    {
        auto elem = *it;
        if (mClock.getElapsedTime() > elem.first)
        {
            // ready to fire, insert them at the beginning
            mQueue.push_front(std::move(elem.second));

            ++it;
            found = true;
        }
        else break;
    }

    if(found)
        mDelayedMessages.erase(mDelayedMessages.begin(), it);
}

bool MessageHandler::isEmpty() const
{
    return mQueue.empty();
}
