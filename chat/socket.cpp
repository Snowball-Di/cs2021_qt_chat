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
    Sleep(7000);    // 等待7秒
    if (wait_flag == true) {
        /*
         * 登录失败处理
         */
        qDebug() << "No response.";
        wait_flag = false;
        return false;
    }
    return true;
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

