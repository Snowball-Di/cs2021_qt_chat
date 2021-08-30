#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <socket.h>
#include <c2s.h>

/*
 * 客户端类
 * 单例模式
 */
class Client : public QObject
{
    Q_OBJECT
public:
    static Client* client_init();

    void userRegister(QString name, QString password);
    void login(int usrID, QString password);

    void sendText(int groupID, QString text);

    void sendFriendRequest(int friendID);
    void acceptFriendRequest();

    void newGroup(QString groupName);
    void joinGroup(int groupID);
    void acceptJoinGroup(int groupID);


signals:


private:

    explicit Client(QObject *parent = nullptr);

    static Client* client;

    Socket* s;
    int usrID;


    bool waiting(SocketMsg& msg);


    // 需要加入UI
};

#endif // CLIENT_H
