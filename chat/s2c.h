#ifndef MESSAGEFROMSERVER_H
#define MESSAGEFROMSERVER_H

#include <QObject>
#include <QDebug>
/*Server to Client*/
namespace S2C {

const int SERVER_REPLY                  =0x01;
const int SERVER_REPLY_REGISTER         =0x02;

const int SERVER_NEWFRIEND              =0x03;
const int SERVER_NEWGROUP               =0x04;
const int SERVER_NEWJOIN                =0x05;
const int SERVER_JOINOK                 =0x06;

const int SERVER_MSG_TEXT               =0x08;
const int SERVER_MSG_DOC                =0x09;

const int SERVER_FRIENDLIST             =0x0A;
const int SERVER_GROUPLIST              =0x0B;
const int SERVER_TEXTRECORD             =0x0C;

const int SERVER_WAITING_FRIEND         =0x0D;
const int SERVER_WAITING_GROUP          =0x0E;

const int SERVER_LATEST_MSG_TIME        =0x0F;


struct Text
{
    int type;
    int senderID;
    int groupID;
    time_t sendTime;
    char senderName[30];
    char text[100];
};

struct Response
{
    int type;
    bool success;
    char text[60];
};

struct Register
{
    int type;
    bool success;
    int usrID;
    char text[60];
};




/*new friend*/
struct NewFriend
{
    int type;
    int senderID;
    char senderName[30];
    char text[50];
};


/*new Group,newJoin,joinOK*/
struct NewGroup
{
    int type;
    int groupID;
    char groupName[30];
};


struct NewJoin
{
    int type;
    int senderID;
    char senderName[30];
    int groupID;
    char groupName[30];
    char text[50];
};


// 好友和群组列表
struct PersonInfo
{
    int personID;
    int groupID;
    char personName[30];
};

struct GroupInfo
{
    int groupID;
    char groupName[30];
};

//等待验证的新好友列表和等待验证的群新成员列表
struct NewFriendInfo{
    int personID;
    char personName[30];
    char text[50];
};

struct NewJoinInfo
{
    int senderID;
    int groupID;
    char groupName[30];
    char senderName[30];
    char text[50];
};

/*friend list*/
struct FriendList
{
    int type;
    bool success;
    int size; // 好友总数
    PersonInfo friends[10];//最多100个好友，从0开始
};

struct NewFriendWaiting//等待验证的新好友
{
    int type;
    bool success;
    int size;
    NewFriendInfo friends[10];
};

/*group list*/
struct GroupList
{
    int type;
    bool success;
    int size; // 群组总数
    GroupInfo groups[20];
};

struct NewJoinWaiting
{
    int type;
    bool success;
    int size;
    NewJoinInfo members[20];
};

/*message record*/
struct NewMesList{
    int groupID;
    bool isfriend;
    bool ifnew;
};

struct Time{
    int type;
    bool success;
    int size;
    NewMesList group[20];
};//各个群是否有新消息，ifnew[i]=true时有新消息

struct Message{
    time_t time;
    int senderID;
    char senderName[30];
    char content[100];
};

struct Record{
    int type;
    bool success;
    int messageNumber;
    Message history[10];
};

};

#endif // MESSAGEFROMSERVER_H
