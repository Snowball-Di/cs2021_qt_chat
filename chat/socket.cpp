#include "socket.h"

Socket* Socket::socket = nullptr;

Socket::Socket(const int _usrID, const int _password) : usrID(_usrID), password(_password)
{
    this->setSocketDescriptor(_usrID);
    connect(this, &Socket::disconnected, this, &Socket::slot_disconnect);
    connect(this, &Socket::readyRead, this, &Socket::slot_readData);
}

Socket* Socket::getSocket(const int _usrID, const int _password)
{
    if (socket == nullptr) {
        socket = new Socket(_usrID, _password);
        connect(socket, &Socket::disconnected, socket, &Socket::slot_disconnect);
        connect(socket, &Socket::readyRead, socket, &Socket::slot_readData);
    }

    return socket;
}


void Socket::slot_disconnect()
{
    emit sig_disconnect(usrID);
}

void Socket::slot_readData()
{
    QByteArray data = readAll();
    emit sig_readyRead(usrID, data);
}

