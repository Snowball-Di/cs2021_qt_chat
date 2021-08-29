#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <message.h>
#include <QHostAddress>
#include "s2c.h"

/*
 * 此类继承自QTcpSocket，提供了标识自身的功能
 */

class Socket : public QTcpSocket
{
    Q_OBJECT
public:

    // 通过该函数获取单例
    static Socket* getSocket(const int _usrID);

signals:
    void clientMessage();


private slots:
    void serverMessageHandler();
    void disconnectHandler();

private:
    explicit Socket(const int _usrID);

    static Socket* socket;
    int usrID;

    bool login(QString password);
    bool waiting;
};


#endif // SOCKET_H
