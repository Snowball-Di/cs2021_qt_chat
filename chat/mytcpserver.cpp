#include "mytcpserver.h"
MyTcpServer* MyTcpServer::tcpserverhand = nullptr;
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    usr=0;
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

    qDebug()<<"someone connected!"<<tcp_socket->peerAddress();
    qDebug()<<"there are "<<clientList.count()<<"user online !";
}
void MyTcpServer::slot_readdata(){
    QByteArray databyte = tcp_socket->readAll();
    char* data=databyte.data();
    C2S::Message* mes = (C2S::Message*) (data);
    int mestype = mes->type();
    switch (mestype){
    case C2S::MSG_REGISTER:
        usrID = (int)Qt_chat_sql::register(mes->sender(),mes->getPassord());
        clientList.insert(usrID,tcp_socket);
        break;
    case C2S::MSG_LOG:

        break;
    }
}

