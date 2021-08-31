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
    // Group g1 = {1111, "你好"};
    // Group g2 = {1211, "欢迎"};

    // QVector<Group> g;
    // g.append(g1);
    // g.append(g2);
    // Manager* m = Manager::getManager(123, true);
    // m->setGroups(g);
    // qDebug() << m->getGroups()[0].name;

    Client* client = Client::client_init();
    client->slot_register("23123", "123312");
    client->s->connectToHost();
//    client->execute();
    return a.exec();
}
