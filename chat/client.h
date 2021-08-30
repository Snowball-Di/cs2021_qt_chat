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

private slots:
    void slot_register(QString name, QString password);
    void slot_login(int usrID, QString password, bool save);
    void slot_logout();

    void slot_dialog(int groupID);
    void slot_send(int groupID, QString text);

    void slot_friendReq(int friendID, QString verifyText);
    void slot_acceptFriendRequest();

    void slot_newGroup(QString groupName);
    void slot_groupReq(int groupID);
    void slot_acceptReq(bool req);

    void slot_friendList();
    void slot_groupList();

signals:


private:

    explicit Client(QObject *parent = nullptr);

    static Client* client;

    Socket* s;
    int usrID;
    QString usrName;


    bool waiting(SocketMsg& msg);


    // 需要加入UI
};

#endif // CLIENT_H
