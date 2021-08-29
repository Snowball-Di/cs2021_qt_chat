#include "manager.h"
#include <qdebug.h>

Manager* Manager::m = nullptr;

Manager::Manager(QString id) :
    id(id.toInt()), root("records/" + id), usr_name(root + "/usrname.txt"),
    friends_list(root + "/friends.list"), groups_list(root + "/groups.list")
{

}

Manager* Manager::getManager(const int usrID, bool _save_account)
{
    QString id = QString("%1").arg(usrID);

    if (!QFile::exists(id)) {
        QDir dir("records");
        dir.mkdir(id);
        dir.mkdir(id + "/record");

        QFile f1("records/" + id + "/friends.list");
        f1.open(QIODevice::WriteOnly);
        QFile f2("records/" + id + "/usrname.txt");
        f2.open(QIODevice::WriteOnly);
        QFile f3("records/" + id + "/groups.list");
        f3.open(QIODevice::WriteOnly);
        f1.close();
        f2.close();
        f3.close();
    }

    QFile f("records/lastusr");
    f.open(QIODevice::WriteOnly);
    if (_save_account)
        f.write(id.toLatin1());
    f.close();

    return new Manager(id);
}

int Manager::getLastID()
{
    QFile f("records/lastusr");
    f.open(QIODevice::ReadOnly);
    auto id = f.readLine();
    f.close();

    if (id.length() == 0)
        return 0;
    else
        return id.toInt();
}

Manager::~Manager()
{
    usr_name.close();
    friends_list.close();
    groups_list.close();
    delete m;
}


QVector<Friend> Manager::getFriends()
{
    QVector<Friend> friends;
    Friend temp;

    friends_list.open(QIODevice::ReadOnly);
    while (1) {
        auto line = QString(groups_list.readLine());
        if (line.length() == 0)
            return friends;

        auto l = line.split(' ');
        temp.id = l[0].toInt();
        temp.group_id = l[1].toInt();
        temp.name = l[2];
        temp.avatar_path = l[3].remove('\n');
        friends.append(temp);
    }
    friends_list.close();
}


QVector<Group> Manager::getGroups()
{
    QVector<Group> groups;
    Group temp;

    groups_list.open(QIODevice::ReadOnly);

    while (1) {
        auto line = QString(groups_list.readLine());
        if (line.length() == 0)
            return groups;

        auto l = line.split(' ');
        temp.group_id = l[0].toInt();
        temp.name = l[1].remove('\n');
        groups.append(temp);
    }
    groups_list.close();
}


void Manager::setFriends(QVector<Friend> &friends)
{
    QString s;
    char temp[100];
    for (auto &f : friends) {
        sprintf(temp, "%d %d %s %s\n",
                f.id, f.group_id, f.name.toUtf8().data(), f.avatar_path.toUtf8().data());
        s.append(temp);
    }

    friends_list.open(QIODevice::WriteOnly);
    friends_list.write(s.toUtf8());
    friends_list.close();
}


void Manager::setGroups(QVector<Group> &groups)
{
    QString s;
    for (auto &g : groups) {
        s.append(QString("%1 ").arg(g.group_id) + g.name + '\n');
    }

    groups_list.open(QIODevice::WriteOnly);
    groups_list.write(s.toUtf8());
    groups_list.close();
}


