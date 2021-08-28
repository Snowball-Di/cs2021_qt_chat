#ifndef MESSAGEFROMSERVER_H
#define MESSAGEFROMSERVER_H

#include <QObject>

namespace MessageFromServer {
    const int SERVER_MSG_SUCCESS        =0x01;
    const int SERVER_MES_GROUPORFRIEND  =0x02;


    class AbstractMessageFromServer{
    public:
        virtual int type() = 0;
    };


    class Success:virtual public AbstractMessageFromServer{
    private:
        bool yes;
    public:
        Success(bool);
        bool isSuccess{return yes};
        int type(){return SERVER_MSG_SUCCESS;};
    };


    class GrouporFriend:virtual public AbstractMessageFromServer{
    private:
        int ID;
        QString Name;
    public:
        GrouporFriend(int,QString);
        int getID();
        QString getName();
        int type(){return SERVER_MES_GROUPORFRIEND;};
    };


}

#endif // MESSAGEFROMSERVER_H
