#include "u2c.h"

using namespace U2C;

Message::Message(const int _targetID) : targetID(_targetID)
{

}

Login::Login(int _userID, QString _password)
    : Message(_userID),
      password(_password)
{

}

Text::Text(int _targetID, QString _text)
    : Message(_targetID),
      text(_text)
{

}

Request::Request(int _targetID, bool _add, QString _text)
    : Message(_targetID),
      add(_add), text(_text)
{

}

Accept::Accept(int _targetID, QString _kind, bool _accept)
    : Message(_targetID),
      kind(_kind), accept(_accept)
{

}
