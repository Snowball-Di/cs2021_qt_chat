#include "client.h"

Client* Client::client = nullptr;

Client::Client(QObject *parent) : QObject(parent)
{

}

Client* Client::client_init()
{
    if (client == nullptr)
        client = new Client();

    return client;
}
