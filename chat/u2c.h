#ifndef UIMESSAGE_H
#define UIMESSAGE_H

#include <QObject>

namespace U2C {

/*
 * 消息种类列表
 *
 */
const int     MSG_REQUEST     =     0x01;
const int     MSG_TEXT        =     0x02;
const int     MSG_DOC         =     0x03;
const int     MSG_LOGIN       =     0x04;
const int     MSG_GROUP       =     0x05;
const int     MSG_JOIN        =     0x06;
const int     MSG_PROFILE     =     0x07;
const int     MSG_REGISTER    =     0x08;
const int     MSG_ACCEPT      =     0x09;


/*
 * Message
 * 一切UI发出的指令都继承此抽象类
 */
class Message
{
public:
    Message(const int _targetID);
    const int targetID;
    virtual int type() = 0;
};


/*
 * 登录操作
 */
class Login: Message
{
public:
    Login(int userID, QString password);
    int type() { return MSG_LOGIN; };

    const QString password;
};


/*
 * 发送消息
 * targetID 为收信人/群
 * text 为消息内容
 */
class Text: Message
{
public:
    Text(int _targetID, QString _text);
    int type() { return MSG_TEXT; };

    const QString text;
};


/*
 * 添加/删除好友
 * add 为true时，表示添加；返回false表示删除
 * text 验证消息
 */
class Request : Message
{
public:
    Request(int _targetID, bool _add, QString _text);

    const bool add;
    const QString text;
};




}

#endif // UIMESSAGE_H
