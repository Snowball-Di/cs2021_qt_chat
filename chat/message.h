#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>
#include <QFile>
#include <abstractmessage.h>


#define     MSG_REQUEST     0x01
#define     MSG_TEXT        0x02
#define     MSG_DOC         0x03
#define     MSG_LOG         0x04
#define     MSG_GROUP       0x05
#define     MSG_JOIN        0x06
#define     MSG_PROFILE     0x07


class Request : AbstractMessage
{
public:
    Request(int _senderID, int _targetID, QDateTime _sendTime, bool _add, QString _text="");

    int type() { return  MSG_REQUEST; };
    bool isAdd() { return add; };
    QString text() { return  _text; };

private:
    bool add;
    QString _text;
};


class Text : AbstractMessage
{
public:
    Text(int _senderID, int _targetID, QDateTime _sendTime, QString _text);
    int type() { return  MSG_TEXT; };
    QString text() { return  _text; };

private:
    QString _text;
};


class Doc : AbstractMessage
{
public:
    Doc(int _senderID, int _targetID, QDateTime _sendTime, int _type, QString _fileName, QByteArray f);
    int type() { return MSG_DOC; };
    int getfileType() { return fileType; };
    QString getName() { return  name; };
    QByteArray& getFile() { return file; };

private:
    int fileType; // 0: document, 1: image
    QString name;
    QByteArray file;
};

class Log : AbstractMessage
{
public:
    Log(int _usrID, QDateTime _sendTime, bool _login, QString _password="");
    int type() { return MSG_LOG; };
    bool isLogin() { return operation; };
    QString getPassword() { return password; };

private:
    QString password;
    const bool operation; // 0: log out, 1: login
};

class Group : AbstractMessage
{
public:
    Group(int _senderID, QDateTime _sendTime, bool _new, QString _name="");

    int type() { return  MSG_GROUP; };
    bool isNew() { return newGroup; };
    QString getName() { return  name; };

private:
    bool newGroup;
    QString name;
};

class Join : AbstractMessage
{
public:
    Join(int _senderID, int _groupID, QDateTime _sendTime, bool _add, QString _text="");

    int type() { return  MSG_JOIN; };
    int getGroupID() { return target(); };
    bool isJoin() { return join; };
    QString text() { return  _text; };

private:
    bool join;
    QString _text;
};


class Profile : AbstractMessage
{
public:
    Profile(int _senderID, QDateTime _sendTime, QString _name, QByteArray _avatar);

    int type() { return  MSG_PROFILE; };
    QString getName() { return  name; };
    QByteArray getAvatar() { return avatar; };

private:
    QString name;
    QByteArray avatar;
};


#endif // MESSAGE_H
