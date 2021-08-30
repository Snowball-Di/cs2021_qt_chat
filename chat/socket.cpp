#include <windows.h>
#include "socket.h"

Socket* Socket::socket = nullptr;

Socket::Socket()
{
    s = new QTcpSocket(this);
    s->connectToHost("10.194.54.239", 5566);
    connect(s, SIGNAL(disconnected()), this, SLOT(disconnectHandler()));
    connect(s, SIGNAL(readyRead()), this, SLOT(serverMessageHandler()));
}

Socket* Socket::getSocket()
{
    if (socket == nullptr) {
        socket = new Socket();
        qDebug() << "connect to server!";
    }

    return socket;
}


bool Socket::sendMessage(char* msg, int size)
{
    qDebug() << "send";
    qint64 length = s->write(msg, size);
    waiting = true;
    return length != -1;
}

void Socket::serverMessageHandler()
{
    bool local_waiting = waiting;

    char *data = new char[1024];
    memcpy(data, s->readAll().data(), 1024);
    int* type = (int*)data;
    qDebug() << "get msg from server.";

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

