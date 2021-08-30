#include <windows.h>
#include "socket.h"

Socket* Socket::socket = nullptr;

Socket::Socket(const int _usrID) : usrID(_usrID)
{
    this->setSocketDescriptor(_usrID);
    connectToHost(QHostAddress::LocalHost, 5566);
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
    bool send_flag = 0;

    switch (msg->type())
    {
    case C2S::MSG_REQUEST  :
        send_flag = send(msg, sizeof(C2S::Request));
        break;

    case C2S::MSG_TEXT     :
        send_flag = send(msg, sizeof(C2S::Text));
        break;

    case C2S::MSG_DOC      :
        send_flag = send(msg, sizeof(C2S::Doc));
        break;

    case C2S::MSG_LOG      :
        send_flag = send(msg, sizeof(C2S::Log));
        break;

    case C2S::MSG_GROUP    :
        send_flag = send(msg, sizeof(C2S::Group));
        break;

    case C2S::MSG_JOIN     :
        send_flag = send(msg, sizeof(C2S::Join));
        break;

    case C2S::MSG_PROFILE  :
        send_flag = send(msg, sizeof(C2S::Profile));
        break;

    case C2S::MSG_REGISTER :
        send_flag = send(msg, sizeof(C2S::Register));
        break;

    case C2S::MSG_ACCEPT   :
        send_flag = send(msg, sizeof(C2S::Accept));
        break;
    }

    return send_flag;
}

bool Socket::send(C2S::Message* msg, size_t size)
{

    if (write((char *)msg, size) == -1)
        return false;
    emit clientMessage();
    return true;
}



void Socket::serverMessageHandler()
{
    QByteArray array = readAll();
    S2C::Type *msg
            = (S2C::Type*)array.data();

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

