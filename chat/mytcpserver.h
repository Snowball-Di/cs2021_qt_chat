#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include "c2s.h"
#include "s2c.h"
#include "qt_chat_sql.h"
#include <cstring>

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
    QHash <int,QTcpSocket*> clientList;
    QString IP,port;
private slots:
    void slot_creatNewConnection();
    void slot_readdata();

signals:
    void serverMessage();
};

#endif // MYTCPSERVER_H
