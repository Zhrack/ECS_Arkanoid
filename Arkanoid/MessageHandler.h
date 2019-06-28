#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "Message.h"
#include <queue>

enum SendType {
    IMMEDIATE = 0,
    ENQUEUE
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
    void receive(Message&& msg, SendType sendType = SendType::ENQUEUE);

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
    /// <returns></returns>
    Message getNext();

    bool isEmpty() const;

private:
    std::queue<Message> mQueue;
};

#endif // !MESSAGE_HANDLER_H
