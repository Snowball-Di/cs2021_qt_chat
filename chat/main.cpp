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

//    Client* client = Client::client_init();
//    client->s->connectToHost();

    UsrMain m;
    m.show();
    m.showdialog();
    return a.exec();
}
