#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>


class MyTCPserver : public QObject
{
    Q_OBJECT
public:
    explicit MyTCPserver(QObject *parent = nullptr);
    static MyTCPserver* gethand(){
        if(tcpserverhand == nullptr)
            tcpserverhand = new MyTCPserver();
        return tcpserverhand;
    }
    void openTCPserver(QString,QString);
private:
    static MyTCPserver* tcpserverhand;
    QHostInfo hostInfo;
private slots:
    void slot_login(QString usr,QString password);
    void slot_logout(QString usr);
    void slot_new_msg(QString sender,int groupID);
    void slot_register(QString usr,QString password);
    void slot_friendRequest(QString usr, QString target);
    void slot_acceptFriend(QString sender);
    void slot_deleteFriend(QString sender);
    void slot_deleteFriend(QString usr1, QString usr2);
    void slot_newGroup(QString usr, QString groupName);
    void slot_join(QString usr, int groupID);
    void slot_deleteGroup(QString usr, int groupID);
    void slot_setName(int usr, QString name);
    void slot_setAvatar(int usr);
    void slot_setGroupName(int usr, QString groupID, QString newName);
signals:

};

#endif // MYTCPSERVER_H
