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
    delete m;
}


QVector<Friend> Manager::getFriends()
{
    while (isBusy());
    busy = true;
    QVector<Friend> friends;
    Friend temp;

    friends_list.open(QIODevice::ReadOnly);
    while (1) {
        auto line = QString(friends_list.readLine());
        if (line.length() == 0) {
            friends_list.close();
            busy = false;
            return friends;
        }

        auto l = line.split(' ');
        temp.id = l[0].toInt();
        temp.group_id = l[1].toInt();
        temp.name = l[2];
        friends.append(temp);
    }

}


QVector<Group> Manager::getGroups()
{
    while (isBusy());
    busy = true;
    QVector<Group> groups;
    Group temp;

    groups_list.open(QIODevice::ReadOnly);

    while (1) {
        auto line = QString(groups_list.readLine());
        if (line.length() == 0) {
            groups_list.close();
            busy = false;
            return groups;
        }

        auto l = line.split(' ');
        temp.group_id = l[0].toInt();
        temp.name = l[1].remove('\n');
        groups.append(temp);
    }
}


void Manager::setFriends(QVector<Friend> &friends)
{
    while (isBusy());
    busy = true;
    QString s;
    char temp[100];
    for (auto &f : friends) {
        sprintf(temp, "%d %d %s\n\0",
                f.id, f.group_id, f.name.toStdString().c_str());
        s.append(temp);
    }

    friends_list.open(QIODevice::WriteOnly);
    friends_list.write(s.toStdString().c_str());
    friends_list.close();
    busy = false;
}


void Manager::setGroups(QVector<Group> &groups)
{
    while (isBusy());
    busy = true;
    QString s;
    for (auto &g : groups) {
        s.append(QString("%1 ").arg(g.group_id) + g.name + '\n');
    }

    groups_list.open(QIODevice::WriteOnly);
    groups_list.write(s.toStdString().c_str());
    groups_list.close();
    busy = false;
}


QVector<Msg> Manager::getMsg(int groupID)
{
    while (isBusy());
    busy = true;
    QString str = QString("%1").arg(groupID);
    QFile f(root + "/record/" + str);
    QVector<Msg> msg;

    if (!f.exists()) {
        f.open(QIODevice::WriteOnly);
        busy = false;
        f.close();
        return msg;
    }

    f.open(QIODevice::ReadOnly);
    Msg temp;
    while (1) {
        auto line = QString(f.readLine());
        if (line.length() < 8) {
            f.close();
            busy = false;
            return msg;
        }

        auto l = line.split('`');
        temp.senderID = l[0].toInt();
        temp.senderName = l[1];
        temp.sendTime = l[2].toLong();
        temp.text = l[3].remove('\n');
        msg.append(temp);
    }
}


void Manager::setMsg(int groupID, QVector<Msg>& messages)
{
    while (isBusy());
    busy = true;
    QString str = QString("%1").arg(groupID);
    QFile f(root + "/record/" + str);
    f.open(QIODevice::WriteOnly);

    QString s = "";
    for (auto &i : messages) {
        s += QString("%1`").arg(i.senderID);
        s += i.senderName + '`';
        s +=  QString("%1`").arg(i.sendTime);
        s += i.text + '\n';
    }

    f.write(s.toStdString().c_str());
    f.close();
    busy = false;
}

void Manager::appendMsg(int groupID, Msg msg)
{
    while (isBusy());
    busy = true;
    QString str = QString("%1").arg(groupID);
    QFile f(root + "/record/" + str);
    if (f.exists())
        f.open(QIODevice::Append);
    else
        f.open(QIODevice::WriteOnly);

    QString s = "";
    s += QString("%1`").arg(msg.senderID);
    s += msg.senderName + '`';
    s +=  QString("%1`").arg(msg.sendTime);
    s += msg.text + '\n';

    f.write(s.toStdString().c_str());
    f.close();
    busy = false;
}

QString Manager::getFriendName(int id)
{
    auto list = getFriends();
    for (auto &i: list)
        if (i.id == id)
            return i.name;
    return "";
}

int Manager::getFriendGroup(int id)
{
    auto list = getFriends();
    for (auto &i: list)
        if (i.id == id)
            return i.group_id;
    return 0;
}

QString Manager::getName(int groupID, int& friendID)
{
    auto list = getFriends();
    for (auto &i: list)
        if (i.group_id == groupID) {
            friendID = i.id;
            return i.name;
        }

    auto list2 = getGroups();
    for (auto &i: list2)
        if (i.group_id == groupID) {
            friendID = 0;
            return i.name;
        }

    return "Unknown ID";
}
