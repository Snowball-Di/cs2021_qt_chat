#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "socket.h"
#include "c2s.h"
#include "manager.h"

/*
 * 客户端类
 * 单例模式
 */
class Client : public QObject
{
    Q_OBJECT
public:
    static Client* client_init();

    void execute();

private slots:
    void slot_register(QString name, QString password);
    void slot_login(int usrID, QString password, bool save);
    void slot_logout();

    void slot_dialog(int groupID);
    void slot_send(int groupID, QString text);

    void slot_friendReq(int friendID, QString verifyText);

    void slot_newGroup(QString groupName);
    void slot_groupReq(int groupID, QString text);
    void slot_acceptReq(int targetID, bool accept, bool isFriend);

    // 下面两个函数会保存后台数据，不会刷新前台
    void slot_friendList();
    void slot_groupList();

signals:


private:

    explicit Client(QObject *parent = nullptr);

    static Client* client;

    Socket* s;
    Manager* manager;

    int usrID = 0;
    QString usrName;

    bool exit_flag = false;

    bool waiting(SocketMsg& msg);

    QVector<S2C::NewFriendInfo> waitingFriends();
    QVector<S2C::NewJoinInfo> waitingGroups();

    // 在线时，处理三类消息
    void newFriend(SocketMsg &msg);
    void newJoin(SocketMsg &msg);
    void newText(SocketMsg &msg);

    // 需要加入UI
};

#endif // CLIENT_H
