#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QQueue>
#include "s2c.h"
#include "c2s.h"

/*
 * 此类继承自QTcpSocket，提供了标识自身的功能
 */
class Socket : public QTcpSocket
{
    Q_OBJECT
public:

    // 通过该函数获取单例
    static Socket* getSocket(const int _usrID=0);

    // 向服务器发送消息
    bool sendMessage(C2S::Message* msg);

    // 从消息队列取出一条消息
    bool getNextMessage();  /* to be completed. */

    // 发送请求后，通过该函数获取反馈消息
    S2C::Type* getResponse();

    // 发送请求后，通过该函数获取反馈消息
    S2C::Type* nextPendingMessage();

    // 关闭套接字
    void close() { QTcpSocket::close(); };

signals:
    void clientMessage();
    void serverMessage(/* class */);


private slots:
    void serverMessageHandler();
    void disconnectHandler();

private:
    explicit Socket(const int _usrID);

    static Socket* socket;
    int usrID;

    bool send(C2S::Message* msg, size_t size);
    bool waitFor();
    bool wait_flag;

    QQueue<int> serverMsgs;
    int response;
};


#endif // SOCKET_H
