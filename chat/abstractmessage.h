#ifndef ABSTRACTMESSAGE_H
#define ABSTRACTMESSAGE_H

#include <QDateTime>

class AbstractMessage
{
public:
    AbstractMessage(int _senderID, int _targetID, QDateTime _sendTime);

    virtual int type() = 0;
    int sender() { return senderID; };
    int target() { return targetID; };
    QDateTime time() { return sendTime; };

private:
    int senderID;
    int targetID;
    QDateTime sendTime;
};

#endif // ABSTRACTMESSAGE_H
