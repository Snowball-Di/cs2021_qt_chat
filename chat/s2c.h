#ifndef MESSAGEFROMSERVER_H
#define MESSAGEFROMSERVER_H

#include <QObject>
#include <time_t>
#include <QDebug>
/*Server to Client*/
namespace S2C {

const int SERVER_REPLY_LOGIN            =0x01;
const int SERVER_REPLY_REGISTER         =0x02;
const int SERVER_REPLY_FRIENDREQUEST    =0x03;
const int SERVER_REPLY_JOIN             =0x04;
const int SERVER_REPLY_SETNAME          =0x05;
const int SERVER_REPLY_SETAVATAR        =0x06;
const int SERVER_REPLY_SETGROUPNAME     =0x07;


const int SERVER_MSG_TEXT               =0x08;
const int SERVER_MSG_DOC                =0x09;

const int SERVER_NEWFRIEND              =0x0A;
const int SERVER_NEWFRIENDOK            =0x0B;

const int SERVER_NEWGROUP               =0x0C;
const int SERVER_NEWJOIN                =0x0D;
const int SERVER_JOINOK                 =0x0E;

const int SERVER_FRIENDLIST             =0x0F;
const int SERVER_GROUPLIST              =0x10;
const int SERVER_TEXTRECORD_GROUP       =0x11;
const int SERVER_TEXTRECORD_FRIEND      =0x12;

struct Text
{
    int type;
    int senderID;
    int groupID;
    time_t sendTime;
    char text[100];
};

struct Response
{
    int type;
    bool success;
    char text[60];
};




/*new friend*/
struct NewFriend
{
    int type;
    int senderID;
    char senderName[30];
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
    int senderID;
    char senderName[30];
    int groupID;
    char groupName[30];
};


// 好友和群组列表
struct PersonInfo
{
    int type;
    int personID;
    int groupID;
    char personName[30];
};

struct GroupInfo
{
    int groupID;
    char groupName[30];
};

/*friend list*/
struct FriendList
{
    int type;
    int friendID;
    int size; // 好友总数
    PersonInfo friends[20];//最多100个好友，从0开始
};

/*group list*/
struct GroupList
{
    int type;
    int groupID;
    int size; // 群组总数
    GroupInfo groups[10];
};

/*message record*/
//struct GroupTextRecord:virtual public Type{
//private:
//    int lastTxtNum;
//public:
//    GroupMessage_text txt[100];//一次最多100条消息记录，从0开始
//    GroupTextRecord(GroupMessage_text _txt[],int _last);
//    int type()  {   return SERVER_TEXTRECORD_GROUP;};
//    int getTxtNum();//假如不足100条消息记录，返回记录数量，否则返回100
//};
//struct FriendTextRecord:virtual public Type{
//private:
//    int lastTxtNum;
//public:
//    FriendMessage_text txt[100];//一次最多100条消息记录，从0开始
//    FriendTextRecord(FriendMessage_text _txt[],int _last);
//    int type()  {   return SERVER_TEXTRECORD_FRIEND;};
//    int getTxtNum();//假如不足100条消息记录，返回记录数量，否则返回100
//};

};

#endif // MESSAGEFROMSERVER_H
