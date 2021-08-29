#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <c2s.h>
#include <QHostAddress>
#include "messagefromserver.h"

/*
 * 此类继承自QTcpSocket，提供了标识自身的功能
 */

class Socket : public QTcpSocket
{
    Q_OBJECT
public:

    // 通过该函数获取单例
    static Socket* getSocket(const int _usrID);

    bool sendMessage(C2S::Message* msg);

signals:
    void clientMessage();


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
};


#endif // SOCKET_H
