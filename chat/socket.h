#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QQueue>
#include "s2c.h"
#include "c2s.h"


struct SocketMsg
{
    int type;
    char *data;
};



/*
 * 此类继承自QTcpSocket，提供了标识自身的功能
 */
class Socket : public QObject
{
    Q_OBJECT
public:

    // 通过该函数获取单例
    static Socket* getSocket();

    // 向服务器发送消息
    bool sendMessage(char* msg, int length);

    // 从消息队列取出一条消息
    bool getNextMessage();  /* to be completed. */

    // 发送请求后，通过该函数获取反馈消息
    // 如果暂时没有消息，返回的对象type和data均为0
    SocketMsg getResponse();

    // 获取server发来的消息
    // 如果没有消息，返回的对象type和data均为0
    SocketMsg nextPendingMessage();

    // 关闭套接字
    void close() { s->close(); };

signals:
    void serverMessage(/* class */);


private slots:
    void serverMessageHandler();
    void disconnectHandler();

private:
    explicit Socket(QObject *parent=nullptr);

    static Socket* socket;
    QTcpSocket* s;
    int usrID;

    bool waiting = false;
    SocketMsg responseFromServer = {0, 0};

    QQueue<SocketMsg> serverMsgs;
};


#endif // SOCKET_H
