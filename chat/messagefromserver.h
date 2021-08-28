#ifndef MESSAGEFROMSERVER_H
#define MESSAGEFROMSERVER_H

#include <QObject>
#include <QDateTime>

namespace MessageFromServer {
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
    const int SERVER_JOINOK                 =0x0D;

    class Type{
    public:
        virtual int type() = 0;
    };

/*Server Reply whether your behavior succeed*/
    class Success:virtual public Type{
    private:
        bool yes;
    public:
        Success(bool);
        bool isSuccess();
    };


    class Login:virtual public Type,virtual public Success{
    public:
        Login(bool);
        int type(){return SERVER_REPLY_LOGIN;};
    };

    class Register:virtual public Type,virtual public Success{
    public:
        Register(bool);
        int type(){return SERVER_REPLY_REGISTER;};
    };
    class FriendRequest:virtual public Type,virtual public Success{
    public:
        FriendRequest(bool);
        int type(){return SERVER_REPLY_FRIENDREQUEST;};
    };
    class Join:virtual public Type,virtual public Success{
    public:
        Join(bool);
        int type(){return SERVER_REPLY_JOIN;};
    };
    class SetName:virtual public Type,virtual public Success{
    public:
        SetName(bool);
        int type(){return SERVER_REPLY_SETNAME;};
    };
    class SetAvatar:virtual public Type,virtual public Success{
    public:
        SetAvatar(bool);
        int type(){return SERVER_REPLY_SETAVATAR;};
    };
    class SetGroupName:virtual public Type,virtual public Success{
    private:
        QString groupName;
    public:
        SetGroupName(bool,QString);
        int type(){return SERVER_REPLY_SETGROUPNAME;};
        QString getGroupName();
    };

/*Server's message to client B*/
    class AbstractMessage
    {
    public:
        AbstractMessage(int _senderID, int _targetID, QDateTime _sendTime);
        int sender() { return senderID; };
        int target() { return targetID; };
        QDateTime time() { return sendTime; };

    private:
        int senderID;
        int targetID;
        QDateTime sendTime;
    };

    class Text:virtual public Type,virtual public AbstractMessage{
    public:
        Text(int _senderID, int _targetID, QDateTime _sendTime, QString _text);
        int type() { return  SERVER_MSG_TEXT; };
        QString text() { return  _text; };

    private:
        QString _text;
        };

/*new friend*/
    class NewFriend:virtual public Type{
    public:
        NewFriend(int _SenderID, QString _SenderName);
        int type(){ return SERVER_NEWFRIEND;};
        int getSenderID();
        QString getSenderName();

    private:
        int senderID;
        QString senderName;
    };

    class NewFriendOK:virtual public Type,virtual public Success{
    private:
        QString friendName;
    public:
        NewFriendOK(bool,QString);
        int type() {    return SERVER_NEWFRIENDOK;};
        QString getFriendName();
    };

/*new or delete Group,newJoin,joinOK*/
    class NewGroup:virtual public Type,virtual public Success{
    private:
        int groupID;
        QString groupName;
        bool ifnew;
    public:
        NewGroup(bool,int,QString,bool);
        int type() {    return SERVER_NEWGROUP;};
        int getGroupID();
        bool deleteornew(){ return ifnew;};// delete:ifnew=0; new:ifnew=1;
        QString getGroupName();
    };

    class NewJoin:virtual public Type{
    private:
        int senderID;
        QString senderName;
    public:
        NewJoin(int,QString);
        int type() {    return SERVER_NEWJOIN;};
        int getSenderID();
        QString getSenderName();
    };

    class JoinOK:virtual public Type,virtual public Success{
    private:
        QString groupName;
    public:
        JoinOK(bool,QString);
        int type() {    return SERVER_JOINOK;};
        QString getGroupName();
    };

};

#endif // MESSAGEFROMSERVER_H
