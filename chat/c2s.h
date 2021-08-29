#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>
#include <QFile>

namespace C2S {

/*
 * 消息种类列表
 *
 */
const int     MSG_REQUEST     =     0x01;
const int     MSG_TEXT        =     0x02;
const int     MSG_DOC         =     0x03;
const int     MSG_LOG         =     0x04;
const int     MSG_GROUP       =     0x05;
const int     MSG_JOIN        =     0x06;
const int     MSG_PROFILE     =     0x07;
const int     MSG_REGISTER    =     0x08;
const int     MSG_ACCEPT      =     0x09;

/*
 * 抽象基类
 */
class Message
{
public:
    Message(int _senderID, int _targetID, QDateTime _sendTime)
        : senderID(_senderID), targetID(_targetID), sendTime(_sendTime)
    { }

    virtual int type() = 0;
    int sender() { return senderID; };
    int target() { return targetID; };
    QDateTime time() { return sendTime; };

private:
    int senderID;
    int targetID;
    QDateTime sendTime;
};

/*
 * 添加/删除好友
 * isAdd()返回true时，表示添加；返回false表示删除
 * text()返回验证消息
 */
class Request : public Message
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

/*
 * 批准添加好友或加入群
 * kind()返回"group"时，表示同意target加入群；返回"friend"表示同意target加为好友
 * senderID()返回接受请求的群或好友的ID
 * accept()返回是否同意
 */
class Accept : public Message
{
public:
    Accept(int _senderID, int _targetID, QDateTime _sendTime, QString _kind, bool _accept);

    int type() { return  MSG_ACCEPT; };
    QString kind() { return _kind; };
    bool accept() { return _accept; };

private:
    QString _kind;
    bool _accept;
};

/*
 * 发送文本消息
 * text()返回发送的文本
 */
class Text : public Message
{
public:
    Text(int _senderID, int _targetID, QDateTime _sendTime, QString _text);
    int type() { return  MSG_TEXT; };
    QString text() { return  _text; };

private:
    QString _text;
};

/*
 * 发送文件、图片
 * getfileType()返回发送的文件类型
 * getName()返回文件名
 * getFile()返回文件本身
 */
class Doc : public Message
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

/*
 * 登录/登出操作
 * isLogin()返回true时表示登录；返回false表示登出
 * getPassword()返回密码
 */
class Log : public Message
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

/*
 * 注册操作
 * getPassword()返回密码
 */
class Register : public Message
{
public:
    Register(QDateTime _sendTime, QString _password);
    int type() { return MSG_REGISTER; };
    QString getPassword() { return password; };

private:
    QString password;
};

/*
 * 新建/删除群操作
 * isNew()返回true时表示新建群；返回false表示删除群（只有群主有权利删除群）
 * getName()返回群名称
 */
class Group : public Message
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

/*
 * 加入/退出群
 * isJoin()返回true时表示加入群；返回false表示退出群（只有群主和本人有权利使目标退群）
 * getGroupID()返回群ID
 * text()返回加入群时的验证消息（退出群时为空）
 */
class Join : public Message
{
public:
    Join(int _senderID, int _groupID, QDateTime _sendTime, bool _join, QString _text="");

    int type() { return  MSG_JOIN; };
    int getGroupID() { return target(); };
    bool isJoin() { return join; };
    QString text() { return  _text; };

private:
    bool join;
    QString _text;
};

/*
 * 更新个人信息
 * getName()返回新昵称（返回为空时不更新）
 * getAvatar()返回新头像（返回为空值时不更新）
 */
class Profile : public Message
{
public:
    Profile(int _usrID, QDateTime _sendTime, QString _name, QByteArray _avatar);

    int type() { return  MSG_PROFILE; };
    QString getName() { return  name; };
    QByteArray getAvatar() { return avatar; };

private:
    QString name;
    QByteArray avatar;
};

}
#endif // MESSAGE_H
