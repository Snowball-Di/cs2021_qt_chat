#include "client.h"
#include <windows.h>

Client* Client::client = nullptr;

Client::Client(QObject *parent) : QObject(parent)
{
    connect(main_w, SIGNAL(signal_dialog(int)), this, SLOT(slot_dialog(int)));
    // connect(main_w, SIGNAL(signal_func()), this, SLOT(slot_func()));
    connect(main_w, SIGNAL(signal_logout()), this, SLOT(slot_logout()));
    connect(main_w, SIGNAL(signal_friendList()), this, SLOT(slot_friendList()));
    connect(main_w, SIGNAL(signal_groupList()), this, SLOT(slot_groupList()));

    connect(regis, SIGNAL(signal_register(QString, QString)), this, SLOT(slot_register(QString, QString)));
    // connect(regis, SIGNAL(signal_cancel()), this, SLOT(slot_cancel()));

    connect(log_w, SIGNAL(signal_login(int, QString, bool)), this, SLOT(slot_login(int, QString, bool)));
    // connect(log_w, SIGNAL(signal_to_register()), this, SLOT(slot_to_register()));

    for (int i = 0; i < 10; i++)
        connect(chat_w[i], SIGNAL(signal_send(int, QString)), this, SLOT(slot_send(int, QString)));

    connect(acpt, SIGNAL(signal_acceptReq(bool)), this, SLOT(slot_acceptReq(bool)));
    
    
    // 启动UI
    /*
     * 待完成
     */
    int lastId = Manager::getLastID();
    log_w = new LogWindow();
    if(lastId == 0)
        log_w->setUi(lastId, false);
    else
        log_w->setUi(lastId, true);



    s = Socket::getSocket();
}

Client* Client::client_init()
{
    if (client == nullptr)
        client = new Client();

    client->log_w->show();
    return client;
}


void Client::slot_register(QString name, QString password)
{
    C2S::Register msg;
    memcpy(msg.name, name.data(), name.length());
    msg.name[password.length()] = 0;
    memcpy(msg.password, password.data(), password.length());
    msg.password[password.length()] = 0;
    msg.type = C2S::MSG_REGISTER;

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_REPLY_REGISTER, 0};

    if (!waiting(info)) {
        qDebug() << "fail to register.";
        return;
    }

    S2C::Response* res = (S2C::Response*)info.data;
    if (res->success == true) {
        // 注册成功
        QString usrID = res->text;  // 获取ID字符串
        /*
         * 待完成
         */
    } else {
        // 注册失败
        /*
         * 待完成
         */
    }

    qDebug() << "register success.";
    qDebug() << "注册ID：" << res->text;

    delete res;
}

void Client::slot_login(int usrID, QString password, bool save)
{
    C2S::Log msg;
    msg.id = usrID;
    msg.type = C2S::MSG_LOG;
    time(&msg.sendTime);
    msg.operation = true;

    memcpy(msg.password, password.data(), password.length());
    msg.password[password.length()] = 0;

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_REPLY, 0};

    if (!waiting(info)) {
        qDebug() << "fail to login.";
        return;
    }

    S2C::Response* res = (S2C::Response*)info.data;

    if (res->success == true) {
        // 登录成功
        manager = Manager::getManager(usrID, save);     // 文件管理器
        auto waiting_groups = waitingGroups();
        auto waiting_friends = waitingFriends();
        requestfriendList();
        auto friendList = manager->getFriends();
        // 显示waiting_groups/friends
        /*
         * 待完成
         */

        main_w = new UsrMain(this->cli_ui);
        main_w->load_friendlist(friendList);


    } else {
        QMessageBox::information(this->cli_ui, tr("提示"), tr("帐号或密码不正确，请检查"), QMessageBox::Yes);
    }

    qDebug() << "login success: " << res->success;
    qDebug() << "user's name：" << res->text;
    this->usrID = usrID;
    this->usrName = res->text;
    delete res;
}

void Client::slot_send(int groupID, QString text)
{
    C2S::Text msg;
    msg.senderID = usrID;
    msg.type = C2S::MSG_TEXT;
    msg.groupID = groupID;
    time(&msg.sendTime);

    memcpy(msg.text, text.data(), text.length());
    msg.text[text.length()] = 0;

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_REPLY, 0};

    if (!waiting(info)) {
        qDebug() << "fail to send text.";
        return;
    }

    S2C::Response* res = (S2C::Response*)info.data;
    if (res->success == true) {
        // 发送成功
        /*
         * 待完成
         */
    } else {
        // 发送失败
        /*
         * 待完成
         */
    }

    qDebug() << QString("%1 : ").arg(usrID) + res->text;
    delete res;
}

void Client::slot_friendReq(int friendID, QString verifyText)
{
    C2S::Request msg;
    msg.senderID = usrID;
    msg.targetID = friendID;
    msg.type = C2S::MSG_REQUEST;
    msg.add = true;
    time(&msg.sendTime);

    memcpy(msg.text, verifyText.data(), verifyText.length());
    msg.text[verifyText.length()] = 0;

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_REPLY, 0};

    if (!waiting(info)) {
        qDebug() << "fail to send request.";
        return;
    }

    S2C::Response* res = (S2C::Response*)info.data;
    if (res->success == true) {
        // 发送成功
        /*
         * 待完成
         */
    } else {
        // 发送失败
        /*
         * 待完成
         */
    }

    delete res;
}

void Client::slot_acceptReq(bool accept)
{
    accept_flag = accept;
//    C2S::Accept msg;
//    msg.senderID = targetID;
//    msg.targetID = usrID;
//    msg.type = C2S::MSG_ACCEPT;
//    msg.kind = isFriend;
//    msg.accept = accept;
//    time(&msg.sendTime);

//    s->sendMessage((char *)&msg, sizeof(msg));

//    SocketMsg info = {S2C::SERVER_REPLY, 0};

//    if (!waiting(info)) {
//        qDebug() << "fail to accept.";
//        return;
//    }

//    S2C::Response* res = (S2C::Response*)info.data;
//    if (res->success == true) {
//        // 发送成功
//        requestfriendList();
//        /*
//         * 待完成
//         */
//    } else {
//        // 发送失败
//        /*
//         * 待完成
//         */
//    }

//    delete res;
}

void Client::slot_newGroup(QString groupName)
{
    C2S::Group msg;
    msg.type = C2S::MSG_GROUP;
    msg.newGroup = true;
    msg.userID = usrID;
    time(&msg.sendTime);

    memcpy(msg.name, groupName.data(), groupName.length());
    msg.name[groupName.length()] = 0;

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_NEWGROUP, 0};

    if (!waiting(info)) {
        qDebug() << "fail to new group.";
        return;
    }

    S2C::Response* res = (S2C::Response*)info.data;

    if (res->success == true) {
        // 发送成功
        requestgroupList();
        /*
         * 待完成
         */
    } else {
        // 发送失败
        /*
         * 待完成
         */
    }



    delete res;
}

void Client::slot_friendList()
{
    requestfriendList();
    auto fl = manager->getFriends();
}

void Client::slot_groupList()
{
    requestgroupList();
    auto gl = manager->getGroups();
}

void Client::requestfriendList()
{
    C2S::FriendList msg;
    msg.type = C2S::MSG_FRIENDLIST;
    msg.userID = usrID;
    time(&msg.sendTime);

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_FRIENDLIST, 0};

    if (!waiting(info)) {
        qDebug() << "fail to get friend list.";
        return;
    }

    S2C::FriendList* res = (S2C::FriendList*)info.data;

    QVector<Friend> temp;
    for (auto &i : res->friends) {
        temp.append({i.personID, i.groupID, i.personName, ""});
    }
    manager->setFriends(temp);

    delete res;
}

void Client::requestgroupList()
{
    C2S::GroupList msg;
    msg.type = C2S::MSG_GROUPLIST;
    msg.userID = usrID;
    time(&msg.sendTime);

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_GROUPLIST, 0};

    if (!waiting(info)) {
        qDebug() << "fail to get friend list.";
        return;
    }

    S2C::GroupList* res = (S2C::GroupList*)info.data;

    QVector<Group> temp;
    for (auto &i : res->groups) {
        temp.append({i.groupID, i.groupName});
    }
    manager->setGroups(temp);

    delete res;
}


void Client::slot_logout()
{
    C2S::Log msg;
    msg.id = usrID;
    msg.type = C2S::MSG_LOG;
    time(&msg.sendTime);
    msg.operation = false;

    s->sendMessage((char *)&msg, sizeof(msg));
    exit_flag = true;
}


void Client::slot_dialog(int groupID)
{
    C2S::Record msg;
    msg.type = C2S::MSG_RECORD;
    msg.groupID = groupID;
    msg.messageNumber = 10;
    time(&msg.sendTime);
    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_TEXTRECORD_GROUP, 0};

    if (!waiting(info)) {
        qDebug() << "fail to get friend list.";
        return;
    }

    S2C::GroupList* res = (S2C::GroupList*)info.data;

    QVector<Msg> msg = manager->getMsg(groupID);
    // 构建聊天框
    /*
     * 待完成
     */
}


void Client::slot_groupReq(int groupID, QString text)
{
    C2S::Join msg;
    msg.senderID = usrID;
    msg.targetID = groupID;
    msg.type = C2S::MSG_JOIN;
    msg.join = true;
    time(&msg.sendTime);

    memcpy(msg.text, text.data(), text.length());
    msg.text[text.length()] = 0;

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_REPLY, 0};

    if (!waiting(info)) {
        qDebug() << "fail to send request.";
        return;
    }

    S2C::Response* res = (S2C::Response*)info.data;
    if (res->success == true) {
        // 发送成功
        /*
         * 待完成
         */
    } else {
        // 发送失败
        /*
         * 待完成
         */
    }

    delete res;
}

bool Client::waiting(SocketMsg& msg)
{
    time_t begin = 0, now = 0;
    time(&begin);

    while (now - begin <= 7) {
        auto m = s->getResponse();
        if (m.type != 0) {
            qDebug() << m.type;
        }
        if (m.type == msg.type) {
            msg = m;
            return true;
        }
        time(&now);
        Sleep(50);
    }
    return false;
}

void Client::execute()
{
    while (!exit_flag) {
        SocketMsg msg = s->nextPendingMessage();
        if (msg.type != 0) {
            switch (msg.type) {
            case S2C::SERVER_NEWFRIEND :
                newFriend(msg);
                break;
            case S2C::SERVER_NEWJOIN:
                newJoin(msg);
                break;
            case S2C::SERVER_MSG_TEXT:
                newText(msg);
                break;
            }
        }

        Sleep(100);
    }
}

QVector<S2C::NewFriendInfo> Client::waitingFriends()
{
    C2S::WaitingFriends msg;
    msg.userID = usrID;
    msg.type = C2S::MSG_JOIN;
    time(&msg.sendTime);

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_WAITING_FRIEND, 0};
    QVector<S2C::NewFriendInfo> list;

    if (!waiting(info)) {
        qDebug() << "fail to get list.";
        return list;
    }

    S2C::NewFriendWaiting* res = (S2C::NewFriendWaiting*)info.data;
    for (int i = 0; i < res->size; i++)
        list.append(res->friends[i]);

    delete res;
    return list;
}


QVector<S2C::NewJoinInfo> Client::waitingGroups()
{
    C2S::WaitingGroups msg;
    msg.userID = usrID;
    msg.type = C2S::MSG_JOIN;
    time(&msg.sendTime);

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_WAITING_FRIEND, 0};
    QVector<S2C::NewJoinInfo> list;

    if (!waiting(info)) {
        qDebug() << "fail to get list.";
        return list;
    }

    S2C::NewJoinWaiting* res = (S2C::NewJoinWaiting*)info.data;
    for (int i = 0; i < res->size; i++)
        list.append(res->friends[i]);

    delete res;
    return list;
}

void Client::newFriend(SocketMsg &msg)
{
    S2C::NewFriend* res = (S2C::NewFriend*)msg.data;
    // 显示
    /*
     * 待完成
     */
}


void Client::newJoin(SocketMsg &msg)
{
    S2C::NewJoin* res = (S2C::NewJoin*)msg.data;
    // 显示
    /*
     * 待完成
     */
}


void Client::newText(SocketMsg &msg)
{
    S2C::Text* res = (S2C::Text*)msg.data;
    // 显示
    /*
     * 待完成
     */
}

