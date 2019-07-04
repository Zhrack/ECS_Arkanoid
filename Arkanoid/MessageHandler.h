#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "Message.h"
#include <list>

enum SendType {
    IMMEDIATE = 0,
    ENQUEUE,
    DELAYED
};

using DelayedMsg = std::pair<sf::Time, Message>;

struct CustomCompare
{
    bool operator()(const DelayedMsg& lhs, const DelayedMsg& rhs)
    {
        return lhs.first > rhs.first;
    }
};


/// <summary>
/// Gives a messaging system to whoever derives from this.
/// </summary>
class MessageHandler
{
public:
    MessageHandler();
    virtual ~MessageHandler();

    /// <summary>
    /// Insert a new incoming message.
    /// </summary>
    /// <param name="msg">The MSG.</param>
    /// <param name="sendType">Whether it enqueues the message or handle immediatly in the caller thread.</param>
    void receive(Message& msg, SendType sendType = SendType::ENQUEUE, const sf::Time& timeToFire = sf::Time::Zero);

protected:
    /// <summary>
    /// Handles the message.
    /// </summary>
    /// <param name="msg">The MSG.</param>
    /// <returns></returns>
    virtual void handleMessage(Message& msg) = 0;
    /// <summary>
    /// Gets the next message in the queue.
    /// </summary>
    /// <param name="msg">The MSG.</param>
    /// <returns>true if a message is present, false if the queue is empty.</returns>
    bool getNext(Message& msg);

    /// <summary>
    /// Pulls messages from the queue and calls handleMessage.
    /// </summary>
    void pullMessages();

    void getReadyMessages();

    bool isEmpty() const;

private:
    /// <summary>
    /// Queue of arrived messages
    /// </summary>
    std::list<Message> mQueue;

    std::list<DelayedMsg> mDelayedMessages;

    /// <summary>
    /// Needed for delayed messages
    /// </summary>
    sf::Clock mClock;
};

#endif // !MESSAGE_HANDLER_H
