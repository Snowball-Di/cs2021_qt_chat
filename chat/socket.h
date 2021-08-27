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

    static Socket* getSocket(const int _usrID, const int _password);
    void slot_readData();

signals:
    void sig_disconnect(int);
    void sig_readyRead(int, const QByteArray&);

private slots:
    void slot_disconnect();

private:
    explicit Socket(const int _usrID, const int _password);

    static Socket* socket;
    int usrID;
    int password;
};


#endif // SOCKET_H
