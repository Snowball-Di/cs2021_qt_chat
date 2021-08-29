#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <socket.h>
#include <c2s.h>

/*
 * 客户端类
 * 单例模式
 */
class Client : public QObject
{
    Q_OBJECT
public:
    static Client* client_init();

signals:


private:
    explicit Client(QObject *parent = nullptr);

    static Client* client;

    Socket* s;
};

#endif // CLIENT_H
