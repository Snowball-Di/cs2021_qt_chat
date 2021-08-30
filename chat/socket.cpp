#include <windows.h>
#include "socket.h"

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
        if (_usrID != 0)
            socket = new Socket(_usrID);
    }

    return socket;
}

bool Socket::sendMessage(C2S::Message* msg)
{
    bool send_flag = 0, timeout_flag = 0;

    switch (msg->type())
    {
    case C2S::MSG_REQUEST  :
        send_flag = send(msg, sizeof(C2S::Request));
        if (send_flag)
            timeout_flag = waitFor();
        break;

    case C2S::MSG_TEXT     :
        send_flag = send(msg, sizeof(C2S::Text));
        if (send_flag)
            timeout_flag = waitFor();
        break;

    case C2S::MSG_DOC      :
        send_flag = send(msg, sizeof(C2S::Doc));
        if (send_flag)
            timeout_flag = waitFor();
        break;

    case C2S::MSG_LOG      :
        connectToHost(QHostAddress::LocalHost, 5566);
        Sleep(500);
        send_flag = send(msg, sizeof(C2S::Log));
        if (send_flag)
            timeout_flag = waitFor();
        break;

    case C2S::MSG_GROUP    :
        send_flag = send(msg, sizeof(C2S::Group));
        if (send_flag)
            timeout_flag = waitFor();
        break;

    case C2S::MSG_JOIN     :
        send_flag = send(msg, sizeof(C2S::Join));
        if (send_flag)
            timeout_flag = waitFor();
        break;

    case C2S::MSG_PROFILE  :
        send_flag = send(msg, sizeof(C2S::Profile));
        if (send_flag)
            timeout_flag = waitFor();
        break;

    case C2S::MSG_REGISTER :
        send_flag = send(msg, sizeof(C2S::Register));
        if (send_flag)
            timeout_flag = waitFor();
        break;

    case C2S::MSG_ACCEPT   :
        send_flag = send(msg, sizeof(C2S::Accept));
        if (send_flag)
            timeout_flag = waitFor();

        break;
    }

    return send_flag && timeout_flag;
}

bool Socket::send(C2S::Message* msg, size_t size)
{

    if (write((char *)msg, size) == -1)
        return false;
    emit clientMessage();
    return true;
}


bool Socket::waitFor()
{
    wait_flag = true;


    while (wait_flag == true) {
        /*
         * 登录失败处理
         */
        qDebug() << "No response.";
        wait_flag = false;
        Sleep(10);
    }
    return true;
}


void Socket::serverMessageHandler()
{
    QByteArray array = readAll();
    S2C::Type *msg
            = (S2C::Type*)&array;

    switch (msg->type()) {
    case S2C::SERVER_JOINOK:

            break;

    case S2C::SERVER_REPLY_JOIN:

            break;
    }

}


void Socket::disconnectHandler()
{

}

