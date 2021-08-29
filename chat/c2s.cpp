#include "message.h"

using namespace C2S;

Request::Request(int _senderID, int _targetID, QDateTime _sendTime, bool _add, QString _text)
    : AbstractMessage(_senderID, _targetID, _sendTime),
      add(_add), _text(_text)
{

}

Accept::Accept(int _senderID, int _targetID, QDateTime _sendTime, QString _kind)
    : AbstractMessage(_senderID, _targetID, _sendTime),
      _kind(_kind)
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

Register::Register(QDateTime _sendTime, QString _password)
    : AbstractMessage(0, 0, _sendTime),
      password(_password)
{

}

Group::Group(int _usrID, QDateTime _sendTime, bool _new, QString _name)
    : AbstractMessage(_usrID, 0, _sendTime),
      newGroup(_new), name(_name)
{

}

Join::Join(int _senderID, int _groupID, QDateTime _sendTime, bool _join, QString _text)
    : AbstractMessage(_senderID, _groupID, _sendTime),
      join(_join), _text(_text)
{

}


Profile::Profile(int _usrID, QDateTime _sendTime, QString _name, QByteArray _avatar)
    : AbstractMessage(_usrID, 0, _sendTime),
      name(_name), avatar(_avatar)
{

}
