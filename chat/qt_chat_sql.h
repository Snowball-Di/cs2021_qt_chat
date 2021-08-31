#ifndef QT_CHAT_SQL_H
#define QT_CHAT_SQL_H

#include <QWidget>
#include <QSqlDatabase>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QSqlQuery>
#include <QApplication>
#include <QSqlError>

struct USER_INFO{
    int userID;
    QString userName;
    QString password;
    QString address;
};

struct MESSAGE{
    int groupID;
    int senderID;
    QDateTime time;
    QString type;
    QString content;
};

struct GROUP_INFO{
    int groupID;
    int type;
    QString groupName;
    QDateTime time;
    int masterID;
    int friendID;
};

struct FRIEND_REQUEST{
    int senderID;
    int recieverID;
    QString content;
};

struct GROUP_REQUEST{
    int senderID;
    int groupID;
    QString content;
};

struct FRIEND_LIST{
    int groupID;
    int friendID;
};




class Qt_chat_sql : public QWidget
{
    Q_OBJECT
public:
    static Qt_chat_sql* getHand(){
        if(Sqlhand == nullptr)
            Sqlhand = new Qt_chat_sql();
        return Sqlhand;
    }

    bool InitDatabase(void);



    bool login(int userID,QString password,USER_INFO& user_info);//登录
    int Register(QString log_name,QString password);//注册
    int newGroup(bool kind,QString groupName,int usrID);//0:friend  1:group新建群租
    bool addMember(int groupID,int userID);//添加群组成员
    bool deleteMember(int groupID,int userID,int senderID,bool type);//0:删除好友 1:将他人踢出群组 userID为被踢或删的用户
    bool deleteGroup(int groupID,int userID);//删除群组
    int getFriendList(int userID,FRIEND_LIST* friendlist);//获取好友列表  -1:错误
    bool getUser_info(QStringList userIDlist,USER_INFO* user_infoList);//获取用户信息
    bool newMsg(MESSAGE msg);//消息信息
    int groupMembers(int groupID,int* group_members);//获取群组成员
    bool editUser_info(int userID,USER_INFO user_info);//更改用户信息
    int get_history(int limit,int groupID,MESSAGE* messagelist,QDateTime time);//获取消息记录
    int getGroup_info(QStringList groupIDList,GROUP_INFO* groupinfo);//获取群组信息



    bool addGrouprequest(int senderID,int groupID,QString content);//加入未处理的群组请求
    int getGrouprequest(int userID,GROUP_REQUEST* grprequest);//获取该用户的未处理群组申请信息
    bool deleteGrouprequest(int senderID,int groupID);//删除群组请求信息

    bool addFriendrequest(int senderID,int recieverID,QString content);//加入未处理的好友请求
    int getFriendrequest(int userID,FRIEND_REQUEST* frdrequest);//获取该用户的未处理好友申请信息
    bool deleteFriendrequest(int senderID,int recieverID);//删除好友请求信息
    int getgroupOwner(int groupID);//返回群主ID

    QString getUserName(int userID);//返回用户姓名
    QString getGroupName(int groupID);//返回群组姓名



private:
    explicit Qt_chat_sql(QWidget *parent = nullptr);
    static Qt_chat_sql* Sqlhand;
    QSqlDatabase database;
    QSqlQuery sqlquery;

    static int usrID;//顺序编写userID
    static int grpID;//顺序编写groupID

};

#endif // QT_CHAT_SQL_H
