#include "mytcpserver.h"
MyTcpServer* MyTcpServer::tcpserverhand = nullptr;
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
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
void MyTcpServer::slot_creatNewConnection(){
    QTcpSocket* tcp_socket = tcp_server.nextPendingConnection();
    connect(tcp_socket,SIGNAL(clientMessage()),this,SLOT(slot_readdata()));

    clientList[usr]=tcp_socket;
    qDebug()<<"someone connected!"<<tcp_socket->peerAddress();
    qDebug()<<"there are "<<clientList.count()<<"user online !";
}
void MyTcpServer::slot_readdata(){

    QByteArray databyte = tcp_socket->readAll();


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
