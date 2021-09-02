#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "socket.h"
#include "c2s.h"
#include "manager.h"
#include "logwindow.h"
#include "register.h"
#include "usrmain.h"
#include "moredetail.h"
#include "chatwindow.h"
#include "newgroup.h"
#include "listitem.h"
#include "acceptreq.h"
#include "client_ui.h"
#include <QMessageBox>
#include "newfrireq.h"
#include "newingroup.h"
#include "acceptreq.h"

/*
 * 客户端类
 * 单例模式
 */
class Client : public QObject
{
    Q_OBJECT
public:
    static Client* client_init();
    void connectToServer();

private slots:
    void slot_register(QString name, QString password);
    void slot_login(int usrID, QString password, bool save);

    void slot_logout();

    void slot_records(int groupID);
    void slot_dialog(int groupID);
    void slot_send(int groupID, QString text);

    void slot_friendReq(int friendID, QString verifyText);

    void slot_newGroup(QString groupName);
    void slot_groupReq(int groupID, QString text);
    void slot_acceptReq(int senderID, int groupID, bool accept);

    // 下面两个函数会保存后台数据，不会刷新前台
    void slot_friendList();
    void slot_groupList();
    void slot_waitingFriends();
    void slot_waitingGroups();
    void slot_offlineMessage();

    //new
    void slot_to_register();
    void slot_cancel();
    void slot_func();


    void slot_serverHandler(SocketMsg msg);

//signals:
//    void signal_newMessage(QVector<int> groupID);

private:

    explicit Client(QObject *parent = nullptr);

    static Client* client;

    Socket* s;
    Manager* manager;

    int usrID = 0;
    QString usrName;

    bool exit_flag = false;
    bool accept_flag;
    bool init_flag = true;

    QVector<S2C::NewJoinInfo> waitingGroups;
    QVector<S2C::NewFriendInfo> waitingFriends;
    QVector<S2C::NewMesList> waitingText;

    bool waiting(SocketMsg& msg);

    // 在线时，处理三类消息
    void newFriend(int senderID, QString name, QString text);
    void newJoin(int senderID, QString senderName, int groupID, QString groupName, QString text);
    void newText(int groupID);

    void requestfriendList();
    void requestgroupList();

    void getRegister(S2C::Register& msg);
    void getLogin(S2C::Response& res);
    void getFriendReq(S2C::Response& res);
    void getAcceptReq(S2C::Response& res);
    void getNewGroup(S2C::NewGroup& res);
    void getFriendList(S2C::FriendList& res);
    void getGroupList(S2C::GroupList& res);
    void getRecord(S2C::Record& res);
    void getGroupReq(S2C::Response& res);
    void getWaitingFriends(S2C::NewFriendWaiting& res);
    void getWaitingGroups(S2C::NewJoinWaiting& res);
    void getOfflineMessage(S2C::Time& res);
    void getText(S2C::Text& res);

    void initMain();


    // 需要加入UI
    LogWindow *log_w;
    Register *regis;
    UsrMain *main_w;
    QVector<ChatWindow*> chat_w;
    Moredetail *more_func;
    client_ui *cli_ui = new client_ui;

};

#endif // CLIENT_H
