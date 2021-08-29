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
    int time;
    QString type;
    QString content;
};

struct GROUP_INFO{
    int groupID;
    int type;
    QString groupName;
    int
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



    bool login(int userID,QString password,USER_INFO& user_info);//done   登录
    int Register(QString log_name,QString password);//done   注册
    int newGroup(bool kind,QString groupName,int usrID);//0:friend  1:group   done   新建群租
    bool addMember(int groupID,int userID);//done  添加群组成员
    bool deleteMember(int groupID,int userID,int senderID,bool type);//0:删除好友 1:将他人踢出群组   undone
    bool deleteGroup(int groupID,int userID);//done   删除群组
    bool getFriendList(int userID,QStringList& friendlist);//done   获取好友列表
    bool getUser_info(QStringList userIDlist,USER_INFO* user_infoList);//undone    获取用户信息
    bool newMsg(MESSAGE msg);//undone   消息信息
    bool groupMembers(int groupID,int* group_members);//done   获取群组成员
    bool editUser_info(int userID,USER_INFO user_info);//done   更改用户信息
    bool get_history(int limit,int userID,int groupID,MESSAGE* messagelist);//undone   获取历史进路
    bool getGroupList(int userID,QStringList& groupList);//undone     获取群组列表
    bool getGroup_info(int groupID);



private:
    explicit Qt_chat_sql(QWidget *parent = nullptr);
    static Qt_chat_sql* Sqlhand;
    QSqlDatabase database;
    QSqlQuery sqlquery;


    QDate date;
    QTime time;

    QString str_date;
    QString str_timeanddate;

    static int usrID;//顺序编写userID
    static int grpID;//顺序编写groupID
signals:

};

#endif // QT_CHAT_SQL_H
