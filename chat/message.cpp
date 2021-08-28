#include "message.h"

Request::Request(int _senderID, int _targetID, QDateTime _sendTime, bool _add, QString _text)
    : AbstractMessage(_senderID, _targetID, _sendTime),
      add(_add), _text(_text)
{

}

Text::Text(int _senderID, int _targetID, QDateTime _sendTime, QString _text)
    : AbstractMessage(_senderID, _targetID, _sendTime),
      _text(_text)
{

}

Doc::Doc(int _senderID, int _targetID, QDateTime _sendTime, int _type, QString _fileName, QByteArray _file)
    : AbstractMessage(_senderID, _targetID, _sendTime),
      fileType(_type), name(_fileName), file(_file)
{

}

Log::Log(int _usrID, QDateTime _sendTime, bool _login, QString _password)
    : AbstractMessage(_usrID, 0, _sendTime),
      password(_password), operation(_login)
{

}

Group::Group(int _usrID, QDateTime _sendTime, bool _new, QString _name)
    : AbstractMessage(_usrID, 0, _sendTime),
      newGroup(_new), name(_name)
{

}
