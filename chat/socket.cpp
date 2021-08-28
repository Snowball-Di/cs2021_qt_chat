#include "socket.h"
#include <windows.h>

Socket* Socket::socket = nullptr;

Socket::Socket(const int _usrID) : usrID(_usrID)
{
    this->setSocketDescriptor(_usrID);
    connect(this, &Socket::disconnected, this, &Socket::slot_disconnect);
    connect(this, &Socket::readyRead, this, SLOT(serverMessage()));
}

Socket* Socket::getSocket(const int _usrID)
{
    if (socket == nullptr) {
        socket = new Socket(_usrID);
    }

    return socket;
}

bool Socket::login(QString password)
{
    connectToHost(QHostAddress::LocalHost, 5566);
    Log login_msg(usrID, QDateTime::currentDateTime(), true, password);
    write((char*)&login_msg, sizeof(login_msg));

    waiting = true;
    Sleep(7000);    // 等待7秒
    if (waiting == true) {
        /*
         * 登录失败处理
         */
        qDebug() << "Unable to login.";
    }
}

void Socket::serverMessage()
{
    msg = readAll();

}

