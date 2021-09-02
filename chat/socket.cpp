/*
 * @Author: your name
 * @Date: 2021-08-27 19:32:48
 * @LastEditTime: 2021-09-01 01:08:04
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \chat\socket.cpp
 */
#include <windows.h>
#include "socket.h"

Socket* Socket::socket = nullptr;

Socket::Socket(QObject *parent) : QObject(parent)
{
    s = new QTcpSocket(this);
}

Socket* Socket::getSocket()
{
    if (socket == nullptr) {
        socket = new Socket();
        qDebug() << "connect to server!";
    }

    return socket;
}

void Socket::connectToHost()
{
    s->connectToHost("192.168.139.206" , 11111);
    connect(s, SIGNAL(disconnected()), this, SLOT(disconnectHandler()));
    connect(s, SIGNAL(readyRead()), this, SLOT(serverMessageHandler()));
}



bool Socket::sendMessage(char* msg, int size)
{
    waiting = true;
    qint64 length = s->write(msg, size);
    qDebug() << "send: " << length<<msg;

    return length != -1;
//    QString mes = "QWERTYU";
//    qDebug()<<s->write(mes.toLatin1());
//    return 1;
}

void Socket::serverMessageHandler()
{
    char *data = new char[2048];
    memcpy(data, s->readAll().data(), 2048);
    int* type = (int*)data;
    qDebug() << "get msg from server." << *type;

    responseFromServer = {*type, data};
    emit serverMessage(responseFromServer);
}

SocketMsg Socket::getResponse()
{
    auto temp = responseFromServer;
    responseFromServer.data = 0;
    responseFromServer.type = 0;
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

