#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>

#include <QObject>
#include <QTcpSocket>

/*
 * 此类继承自QTcpSocket，提供了标识自身的功能
 *
 */

class Socket : public QTcpSocket
{
    Q_OBJECT
public:

    // 通过该函数获取单例
    static Socket* getSocket(const int _usrID, const QString _password);
    void slot_readData();

signals:
    void clientMessage();

private slots:
    void slot_disconnect();

private:
    explicit Socket(const int _usrID, const QString _password);

    static Socket* socket;
    int usrID;
    QString password;
};


#endif // SOCKET_H
