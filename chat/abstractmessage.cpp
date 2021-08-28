#include "abstractmessage.h"

AbstractMessage::AbstractMessage(int _senderID, int _targetID, QDateTime _sendTime)
    : senderID(_senderID), targetID(_targetID), sendTime(_sendTime)
{

}
