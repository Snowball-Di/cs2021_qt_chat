#include "mainwindow.h"
#include "logwindow.h"
#include "manager.h"
#include "QDebug"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Group g1 = {1111, "你好"};
    Group g2 = {1211, "欢迎"};

    QVector<Group> g;
    g.append(g1);
    g.append(g2);
    Manager* m = Manager::getManager(123, true);
    m->setGroups(g);
    qDebug() << m->getGroups()[0].name;
    return a.exec();
}
