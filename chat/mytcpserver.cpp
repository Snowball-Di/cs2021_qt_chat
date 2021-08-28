#include "mytcpserver.h"
MyTCPserver* MyTCPserver::tcpserverhand = nullptr;
MyTCPserver::MyTCPserver(QObject *parent) : QObject(parent)
{
    QHostAddress hostadd;
    hostadd.setAddress(IP);
    if(tcp_server.listen(hostadd,port.toInt())){
        connect(&tcp_server,SIGNAL(newConnection()),this,SLOT(slot_creatNewConnection()));
    }
    else{
        qDebug()<<"server start error !";
    }
}
void MyTCPserver::slot_creatNewConnection(){
    QTcpSocket* tcp_socket = tcp_server.nextPendingConnection();
    clientList.append(tcp_socket);
    connect(tcp_socket,SIGNAL(readyRead()),this,SLOT(slot_readdata()));
    qDebug()<<"someone connected!"<<tcp_socket->peerAddress();
    qDebug()<<"there are "<<clientList.count()<<"user online !";
}
void MyTCPserver::slot_readdata(){

    QByteArray databyte = tcp_socket->readAll();
    qDebug()<<QString::fromStdString(databyte.toStdString());
    QString str = "I have get your worlds :";
    str.append(QString::fromStdString(databyte.toStdString()));
    tcp_socket->write(str.toLatin1());
}
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
