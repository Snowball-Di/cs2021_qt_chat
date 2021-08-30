#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QFile>

namespace C2S {

/*
 * 消息种类列表
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
 * 添加/删除好友
 * isAdd()返回true时，表示添加；返回false表示删除
 * text()返回验证消息
 */
struct Request
{
    int type;
    int senderID;
    int targetID;
    time_t sendTime;
    bool add;
    char text[50];
};

/*
 * 批准添加好友或加入群
 * kind: 0表示加入群；1表示好友
 * senderID()返回接受请求的群或好友的ID
 * accept()返回是否同意
 */
struct Accept
{
    int type;
    int senderID;
    int targetID;
    time_t sendTime;
    bool kind;
    bool accept;
};


/*
 * 发送文本消息
 * text()返回发送的文本
 */
struct Text
{
    int type;
    int senderID;
    int groupID;
    time_t sendTime;
    char text[100];
};

/*
 * 发送文件、图片
 * getfileType()返回发送的文件类型
 * getName()返回文件名
 * getFile()返回文件本身
 */
struct Doc
{
    int type;
    int senderID;
    int targetID;
    time_t sendTime;
    int fileType; // 0: document, 1: image
    QString name;
    QByteArray file;
};

/*
 * 登录/登出操作
 * isLogin()返回true时表示登录；返回false表示登出
 * getPassword()返回密码
 */
struct Log
{
    int type;
    int id;
    time_t sendTime;
    char password[20];
    bool operation; // 0: log out, 1: login
};

/*
 * 注册操作
 * getPassword()返回密码
 */
struct Register
{
    int type;
    time_t sendTime;
    char name[30];
    char password[20];
};

/*
 * 新建/删除群操作
 * isNew()返回true时表示新建群；返回false表示删除群（只有群主有权利删除群）
 * getName()返回群名称
 */
struct Group
{
    int type;
    int userID;
    time_t sendTime;
    bool newGroup;
    char name[30];
};

/*
 * 加入/退出群
 * isJoin()返回true时表示加入群；返回false表示退出群（只有群主和本人有权利使目标退群）
 * getGroupID()返回群ID
 * text()返回加入群时的验证消息（退出群时为空）
 */
struct Join
{
    int type;
    int senderID;
    int targetID;
    time_t sendTime;
    bool join;
    QString _text;
};

/*
 * 更新个人信息
 * getName()返回新昵称（返回为空时不更新）
 * getAvatar()返回新头像（返回为空值时不更新）
 */
struct Profile
{
    int type;
    int senderID;
    int targetID;
    time_t sendTime;
    QString name;
    QByteArray avatar;
};

}
#endif // MESSAGE_H
