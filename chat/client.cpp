#include "client.h"
#include <windows.h>

Client* Client::client = nullptr;

Client::Client(QObject *parent) : QObject(parent)
{
    log_w = new LogWindow();
    main_w = new UsrMain();
    regis = new Register();
    connect(main_w, SIGNAL(signal_dialog(int)), this, SLOT(slot_dialog(int)));
    connect(main_w, SIGNAL(signal_func()), this, SLOT(slot_func()));
    connect(main_w, SIGNAL(signal_logout()), this, SLOT(slot_logout()));
    connect(main_w, SIGNAL(signal_friendList()), this, SLOT(slot_friendList()));
    connect(main_w, SIGNAL(signal_groupList()), this, SLOT(slot_groupList()));

    connect(regis, SIGNAL(signal_register(QString, QString)), this, SLOT(slot_register(QString, QString)));
    connect(regis, SIGNAL(signal_cancel()), this, SLOT(slot_cancel()));

    connect(log_w, SIGNAL(signal_login(int, QString, bool)), this, SLOT(slot_login(int, QString, bool)));
    connect(log_w, SIGNAL(signal_to_register()), this, SLOT(slot_to_register()));

//    for (int i = 0; i < 10; i++)
//        connect(chat_w[i], SIGNAL(signal_send(int, QString)), this, SLOT(slot_send(int, QString)));

//    connect(acpt, SIGNAL(signal_acceptReq(bool)), this, SLOT(slot_acceptReq(bool)));
    
    
    // 启动UI
    /*
     * 待完成
     */
    int lastId = Manager::getLastID();

    if(lastId != 0)
        log_w->setUi(lastId, false);

    s = Socket::getSocket();
}

Client* Client::client_init()
{
    if (client == nullptr)
        client = new Client();

    client->log_w->show();
    return client;
}

void Client::slot_to_register()
{
    log_w->hide();
    regis->show();
}

void Client::slot_cancel()
{
    regis->close();
    log_w->show();
}

void Client::slot_func()
{
    more_func = new Moredetail(this->cli_ui);
    QString name = usrName;
    int usr_id = usrID;
    more_func->setUser(name, QString("%1").args(usr_id), QPixmap(":/img/img/log_icon.png"));
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
        QMessageBox::information(this->cli_ui, tr("账号信息"), QString("您的帐号为：")+usrID, QMessageBox::Yes);
        log_w->setUi(usrID.toInt(), false);
        regis->close();
        log_w->show();
        qDebug() << "register success.";
        qDebug() << "注册ID：" << res->text;
    } else {
        // 注册失败
       QMessageBox::information(this->cli_ui, tr("注册失败"), QString("请重试！")+usrID, QMessageBox::Yes);
       qDebug() << "register fail.";
       qDebug() << "注册ID：" << res->text;
    }



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
        auto msgs = getOfflineMessage();

         for (auto &i : waiting_groups)
            newJoin(i.senderID, i.senderName, i.groupID, i.text);

         for (auto &i : waiting_friends)
            newFriend(i.personID, i.personName, i.text);

         for (auto &i : msgs) {
             if (i.ifnew)
                newText(i.groupID);
         }

//        //mock
//         QVector<Friend> fri(5);
//         fri[0].name = "Max";
//         fri[2].name = "Max2341234";
//         fri[1].name = "Max21";
//         fri[3].name = "我是呆瓜";
//         fri[4].name = "另一个呆瓜";


        main_w->load_friendlist(friendList);
        main_w->show();
        log_w->close();

        qDebug() << "login success: " << res->success;
        qDebug() << "user's name：" << res->text;
    } else {
        QMessageBox::information(this->cli_ui, tr("提示"), tr("帐号或密码不正确，请检查"), QMessageBox::Yes);
        qDebug() << "login fail: " << res->success;
        qDebug() << "user's name：" << res->text;
    }

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
        QMessageBox::information(this->more_func, tr("success"), tr(res->text), QMessageBox::Yes);
    } else {
        // 发送失败
        QMessageBox::information(this->more_func, tr("success"), tr(res->text), QMessageBox::Yes);
    }

    delete res;
}

void Client::slot_acceptReq(int senderID, int groupID, bool accept)
{
    C2S::Accept msg;
    msg.senderID = senderID;
    msg.targetID = (groupID == 0)? usrID : groupID;
    msg.type = C2S::MSG_ACCEPT;
    msg.kind = (groupID == 0);
    msg.accept = accept;
    time(&msg.sendTime);

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_REPLY, 0};

    if (!waiting(info)) {
        qDebug() << "fail to accept.";
        return;
    }

    S2C::Response* res = (S2C::Response*)info.data;
    if (res->success == true) {
        requestfriendList();
        // reload friend list
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
        QMessageBox::information(this->more_func, tr("success"), tr(res->text), QMessageBox::Yes);
    } else {
        // 发送失败
        QMessageBox::information(this->more_func, tr("fail"), tr(res->text), QMessageBox::Yes);
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

    SocketMsg info = {S2C::SERVER_TEXTRECORD, 0};

    if (!waiting(info)) {
        qDebug() << "fail to get friend list.";
        return;
    }

    S2C::Record* res = (S2C::Record*)info.data;
    QVector<Msg> temp;
    for (int i = 0; i < res->messageNumber; i++)
        temp.append({res->history[i].senderID, res->history[i].senderName,
                     res->history[i].time, res->history[i].content});
    manager->setMsg(groupID, temp);

    QVector<Msg> msgList = manager->getMsg(groupID);
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
        QMessageBox::information(this->more_func, tr("success"), tr(res->text), QMessageBox::Yes);
    } else {
        // 发送失败
        QMessageBox::information(this->more_func, tr("success"), tr(res->text), QMessageBox::Yes);
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
            if (msg.type == S2C::SERVER_NEWFRIEND) {
                S2C::NewFriend* res = (S2C::NewFriend*)msg.data;
                newFriend(res->senderID, res->senderName, res->text);
            } else if (msg.type == S2C::SERVER_NEWJOIN) {
                S2C::NewJoin* res = (S2C::NewJoin*)msg.data;
                newJoin(res->senderID, res->senderName, res->groupID, res->text);
            } else if (msg.type == S2C::SERVER_MSG_TEXT) {
                S2C::Text* res = (S2C::Text*)msg.data;
                auto m = manager->getMsg(res->groupID);
                m.append({res->senderID, manager->getFriendName(res->senderID), res->sendTime, res->text});
                manager->setMsg(res->groupID, m);
                newText(res->groupID);
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
        list.append(res->members[i]);

    delete res;
    return list;
}

void Client::newFriend(int senderID, QString name, QString text)
{
    // 显示
    /*
     * 待完成
     */
    // senderID, groupID = 0

}


void Client::newJoin(int senderID, QString name, int groupID, QString text)
{
    // 显示
    /*
     * 待完成
     */
    // senderID, groupID != 0
}


void Client::newText(int groupID)
{

    if (1/*窗口active*/) {

    } else {

    }
    // 显示
    /*
     * 待完成
     */
}

QVector<S2C::NewMesList> Client::getOfflineMessage()
{
    C2S::Time msg;
    msg.userID = usrID;
    msg.type = C2S::MSG_TIME;
    time(&msg.sendTime);

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_LATEST_MSG_TIME, 0};
    QVector<S2C::NewMesList> list;

    if (!waiting(info)) {
        qDebug() << "fail to get list.";
        return list;
    }

    S2C::Time* res = (S2C::Time*)info.data;
    for (int i = 0; i < res->size; i++)
        list.append(res->group[i]);

    delete res;
    return list;
}
