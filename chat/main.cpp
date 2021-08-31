#include "chatwindow.h"
#include "logwindow.h"
#include "manager.h"
#include "QDebug"
#include "usrmain.h"
#include <QApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Client* client = Client::client_init();
    client->slot_register("23123", "123312");
    client->s->connectToHost();
//    client->execute();
    return a.exec();
}
