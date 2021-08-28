#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    static MyTcpServer* gethand(){
        if(tcpserverhand == nullptr)
            tcpserverhand = new MyTcpServer();
        return tcpserverhand;
    }
    void openTCPserver(QString,QString);
private:
    static MyTcpServer* tcpserverhand;
    QHostInfo hostInfo;
    QTcpServer tcp_server;
    QTcpSocket* tcp_socket;
    //QList<QTcpSocket*> clientList;
    QHash <QTcpSocket,int> clientList;

    QString password,target,groupName;
    QString IP,port;
    int usr,usr1,usr2,sender,groupID;
private slots:
    void slot_creatNewConnection();
    void slot_readdata();

    void slot_login(int usr,QString password);
    void slot_logout(int usr);
    void slot_new_msg(int sender,int groupID);
    void slot_register(int usr,QString password);
    void slot_friendRequest(int usr, QString target);
    void slot_acceptFriend(int sender);
    void slot_deleteFriend(int sender);
    void slot_deleteFriend(int usr1, int usr2);
    void slot_newGroup(int usr, QString groupName);
    void slot_join(int usr, int groupID);
    void slot_deleteGroup(int usr, int groupID);
    void slot_setName(int usr, QString name);
    void slot_setAvatar(int usr);
    void slot_setGroupName(int usr, int groupID, QString groupName);
signals:

};

#endif // MYTCPSERVER_H
