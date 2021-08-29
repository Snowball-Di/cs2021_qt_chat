#include <windows.h>
#include "socket.h"

#define     mfs     MessageFromServer

Socket* Socket::socket = nullptr;

Socket::Socket(const int _usrID) : usrID(_usrID)
{
    this->setSocketDescriptor(_usrID);
    connect(this, &Socket::disconnected, this, SLOT(disconnectHandler()));
    connect(this, &Socket::readyRead, this, SLOT(serverMessageHandler()));
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
    emit clientMessage();

    waiting = true;
    Sleep(7000);    // 等待7秒
    if (waiting == true) {
        /*
         * 登录失败处理
         */
        qDebug() << "Unable to login.";
    }
}

void Socket::serverMessageHandler()
{
    QByteArray array = readAll();
    mfs::AbstractMessageFromServer *msg
            = (mfs::AbstractMessageFromServer*)&array;

    switch (msg->type()) {
        case mfs::SERVER_MSG_SUCCESS:

            break;

        case mfs::SERVER_MES_GROUPORFRIEND:

            break;
    }

}

void Socket::disconnectHandler()
{
    
}

