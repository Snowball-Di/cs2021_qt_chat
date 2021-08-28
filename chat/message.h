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
#define     MSG_REGISTER    0x08
#define     MSG_ACCEPT      0x09


/*
 * 添加/删除好友
 * isAdd()返回true时，表示添加；返回false表示删除
 * text()返回验证消息
 */
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

/*
 * 添加/删除好友
 * kind()返回"group"时，表示同意target加入群；返回"friend"表示同意target加为好友
 * senderID()返回接受请求的群或好友的ID
 */
class Accept : AbstractMessage
{
public:
    Accept(int _senderID, int _targetID, QDateTime _sendTime, QString _kind);

    int type() { return  MSG_ACCEPT; };
    QString kind() { return _kind; };

private:
    QString _kind;
};

/*
 * 发送文本消息
 * text()返回发送的文本
 */
class Text : AbstractMessage
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

/*
 * 登录/登出操作
 * isLogin()返回true时表示登录；返回false表示登出
 * getPassword()返回密码
 */
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

/*
 * 注册操作
 * getPassword()返回密码
 */
class Register : AbstractMessage
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

/*
 * 加入/退出群
 * isJoin()返回true时表示加入群；返回false表示退出群（只有群主和本人有权利使目标退群）
 * getGroupID()返回群ID
 * text()返回加入群时的验证消息（退出群时为空）
 */
class Join : AbstractMessage
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
class Profile : AbstractMessage
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


#endif // MESSAGE_H
