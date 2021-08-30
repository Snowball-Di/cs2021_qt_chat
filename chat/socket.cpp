#include <windows.h>
#include "socket.h"

Socket* Socket::socket = nullptr;

Socket::Socket()
{
    connectToHost(QHostAddress::LocalHost, 5566);
    connect(this, &Socket::disconnected, this, SLOT(disconnectHandler()));
    connect(this, &Socket::readyRead, this, SLOT(serverMessageHandler()));
}

Socket* Socket::getSocket()
{
    if (socket == nullptr) {
        socket = new Socket();
    }

    return socket;
}


bool Socket::sendMessage(char* msg, int size)
{
    qint64 length = write(msg, size);
    emit clientMessage();
    waiting = true;
    return length != -1;
}

void Socket::serverMessageHandler()
{
    bool local_waiting = waiting;

    char *data = new char[1024];
    memcpy(data, socket->readAll().data(), 1024);
    int* type = (int*)data;

    if (local_waiting) {
        waiting = false;
        responseFromServer = {*type, data};
    } else {
        serverMsgs.enqueue({*type, data});
    }
}

SocketMsg Socket::getResponse()
{
    auto temp = responseFromServer;
    responseFromServer.data = 0;
    return temp;
}


SocketMsg Socket::nextPendingMessage()
{
    if (serverMsgs.empty())
        return {0, 0};

    return serverMsgs.dequeue();
}

void Socket::disconnectHandler()
{

}

