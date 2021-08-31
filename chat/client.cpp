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
    connect(s, SIGNAL(serverMessage(SocketMsg)), this, SLOT(slot_serverHandler(SocketMsg)));
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
    more_func->setUser(name, QString("%1").arg(usr_id), QPixmap(":/img/img/log_icon.png"));
}


void Client::slot_serverHandler(SocketMsg msg)
{
    if (msg.type == S2C::SERVER_REPLY_REGISTER) {
        getRegister(*(S2C::Register *)msg.data);
    } else if (msg.type == S2C::SERVER_REPLY) {

    } else if (msg.type == S2C::SERVER_REPLY_REGISTER) {

    }
    else if (msg.type == S2C::SERVER_NEWFRIEND) {

    }
    else if (msg.type == S2C::SERVER_NEWGROUP) {

    }
    else if (msg.type == S2C::SERVER_NEWJOIN) {

    }
    else if (msg.type == S2C::SERVER_JOINOK) {

    }
    else if (msg.type == S2C::SERVER_MSG_TEXT) {
        getText(*(S2C::Text *)msg.data);
    }
    else if (msg.type == S2C::SERVER_MSG_DOC) {

    }
    else if (msg.type == S2C::SERVER_FRIENDLIST) {

    }
    else if (msg.type == S2C::SERVER_GROUPLIST) {

    }
    else if (msg.type == S2C::SERVER_TEXTRECORD) {

    }
    else if (msg.type == S2C::SERVER_WAITING_FRIEND) {

    }
    else if (msg.type == S2C::SERVER_WAITING_GROUP) {

    }
    else if (msg.type == S2C::SERVER_LATEST_MSG_TIME) {

    }
}

void Client::slot_register(QString name, QString password)
{
    C2S::Register msg;
    auto n = name.toLocal8Bit();
    auto p = password.toLocal8Bit();
    memcpy(msg.name, n.data(), n.length());
    msg.name[n.length()] = 0;
    memcpy(msg.password, p.data(), p.length());
    msg.password[p.length()] = 0;
    msg.type = C2S::MSG_REGISTER;

    s->sendMessage((char *)&msg, sizeof(msg));
}

void Client::getRegister(S2C::Register& msg)
{

    if (msg.success == true) {
        // 注册成功
        int usrID = msg.usrID;  // 获取ID字符串
        QMessageBox::information(this->cli_ui, tr("账号信息"),
                                 QString("您的帐号为：")+QString("%1").arg(usrID), QMessageBox::Yes);
        log_w->setUi(usrID, false);
        regis->close();
        log_w->show();
        qDebug() << "register success.";
    } else {
        // 注册失败
       QMessageBox::information(this->cli_ui, tr("注册失败"), QString("请重试！")+usrID, QMessageBox::Yes);
       qDebug() << "register fail.";
    }
}

static bool save_flag;

void Client::slot_login(int usrID, QString password, bool save)
{
    save_flag = save;
    C2S::Log msg;
    msg.id = usrID;
    msg.type = C2S::MSG_LOG;
    time(&msg.sendTime);
    msg.operation = true;

    auto p = password.toStdString();
    memcpy(msg.password, p.c_str(), p.length());
    msg.password[p.length()] = 0;

    this->usrID = usrID;
    s->sendMessage((char *)&msg, sizeof(msg));
}


void Client::getLogin(S2C::Response& res)
{
    if (res.success == true) {
        // 登录成功
        manager = Manager::getManager(usrID, save_flag);     // 文件管理器
        // 反复请求数据
        requestgroupList();
    } else {
        QMessageBox::information(this->cli_ui, tr("提示"), tr("帐号或密码不正确，请检查"), QMessageBox::Yes);
        qDebug() << "login fail: " << res.success;
        qDebug() << "user's name：" << res.text;
    }
    this->usrName = res.text;
}


void Client::initMain()
{
    // 展示前端
    auto friendList = manager->getFriends();

    main_w->load_friendlist(friendList);
    main_w->show();
    log_w->close();

    qDebug() << "login success" ;
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
}

void Client::getText(S2C::Text& res)
{
    auto msgs = manager->getMsg(res.groupID);
    Msg m;
    m.text = res.text;
    m.sendTime = res.sendTime;
    m.senderID = res.senderID;
//    m.senderName = res.name;
    msgs.append(m);
    manager->setMsg(res.groupID, msgs);
    newText(res.groupID);
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
}

void Client::getFriendReq(S2C::Response& res)
{
    if (res.success == true) {
        // 发送成功
        QMessageBox::information(this->more_func, tr("success"), tr(res.text), QMessageBox::Yes);
    } else {
        // 发送失败
        QMessageBox::information(this->more_func, tr("success"), tr(res.text), QMessageBox::Yes);
    }
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
}

void Client::getAcceptReq(S2C::Response& res)
{
    if (res.success == true) {
        // reload friend list
        QMessageBox::information(this->main_w, tr("success"), tr(res.text), QMessageBox::Yes);
        // 请求好友列表
//        QVector<Friend> temp_friends = manager->getFriends();
//        this->main_w->load_friendlist(temp_friends);
        requestfriendList();
    } else {
        // 发送失败
        QMessageBox::information(this->main_w, tr("fail"), tr(res.text), QMessageBox::Yes);
    }
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
}

void Client::getNewGroup(S2C::Response& res)
{
    if (res.success == true) {
        // 发送成功
        QMessageBox::information(this->more_func, tr("success"), tr(res.text), QMessageBox::Yes);
        requestgroupList();
    } else {
        // 发送失败
        QMessageBox::information(this->more_func, tr("fail"), tr(res.text), QMessageBox::Yes);
    }
}

static bool showGroup = false;
static bool showFriend = true;
void Client::slot_friendList()
{
    showGroup = false;
    showFriend = true;
    requestfriendList();
}

void Client::slot_groupList()
{
    showGroup = true;
    showFriend = false;
    requestgroupList();
}

void Client::requestfriendList()
{
    C2S::FriendList msg;
    msg.type = C2S::MSG_FRIENDLIST;
    msg.userID = usrID;
    time(&msg.sendTime);

    s->sendMessage((char *)&msg, sizeof(msg));
}


void Client::getFriendList(S2C::FriendList& res)
{
    QVector<Friend> temp;
    for (auto &i : res.friends) {
        temp.append({i.personID, i.groupID, i.personName, ""});
    }
    manager->setFriends(temp);


    if (showFriend) {
        // ui显示
        this->main_w->load_friendlist(temp);
        this->main_w->show();
    }

    if (init_flag) {
        slot_waitingGroups();
    }
}

void Client::requestgroupList()
{
    C2S::GroupList msg;
    msg.type = C2S::MSG_GROUPLIST;
    msg.userID = usrID;
    time(&msg.sendTime);

    s->sendMessage((char *)&msg, sizeof(msg));
}


void Client::getGroupList(S2C::GroupList& res)
{
    QVector<Group> temp;
    for (auto &i : res.groups) {
        temp.append({i.groupID, i.groupName});
    }
    manager->setGroups(temp);

    if (showGroup) {
        // ui显示
        this->main_w->load_grouplist(temp);
        this->main_w->show();
    }
    if (init_flag) {
        requestfriendList();
    }
}


void Client::slot_logout()
{
    C2S::Log msg;
    msg.id = usrID;
    msg.type = C2S::MSG_LOG;
    time(&msg.sendTime);
    msg.operation = false;

    s->sendMessage((char *)&msg, sizeof(msg));
}

static int groupID_dialog = 0;

void Client::slot_dialog(int groupID)
{
    C2S::Record msg;
    groupID_dialog = groupID;
    msg.type = C2S::MSG_RECORD;
    msg.groupID = groupID;
    msg.messageNumber = 10;
    time(&msg.sendTime);
    s->sendMessage((char *)&msg, sizeof(msg));
}

void Client::getDialog(S2C::Record& res)
{
    QVector<Msg> temp;
    for (int i = 0; i < res.messageNumber; i++)
        temp.append({res.history[i].senderID, res.history[i].senderName,
                     res.history[i].time, res.history[i].content});
    manager->setMsg(groupID_dialog, temp);

    QVector<Msg> msgList = manager->getMsg(groupID_dialog);

    for(int i = 0; i < chat_w.length(); i++)
    {
        if (chat_w[i]->groupid == groupID_dialog)
        {
            chat_w[i]->loadMessageHis(msgList, usrID);
            chat_w[i]->show();
            return;
        }
    }

    ChatWindow *temp_w = new ChatWindow(this->main_w);
    connect(temp_w, SIGNAL(signal_send(int, QString)), this, SLOT(slot_send(int, QString)));
    connect(temp_w, SIGNAL(signal_send(int, QString)), this, SLOT(slot_send(int, QString)));
    temp_w->groupid = groupID_dialog;
    // todo 显示对方信息
    temp_w->show();
    chat_w.append(temp_w);
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
}

void Client::getGroupReq(S2C::Response& res)
{
    if (res.success == true) {
        // 发送成功
        QMessageBox::information(this->more_func, tr("success"), tr(res.text), QMessageBox::Yes);
    } else {
        // 发送失败
        QMessageBox::information(this->more_func, tr("success"), tr(res.text), QMessageBox::Yes);
    }
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

void Client::slot_waitingFriends()
{
    C2S::WaitingFriends msg;
    msg.userID = usrID;
    msg.type = C2S::MSG_WAITINNG_FRIEND;
    time(&msg.sendTime);

    s->sendMessage((char *)&msg, sizeof(msg));
}

void Client::getWaitingFriends(S2C::NewFriendWaiting& res)
{
    for (int i = 0; i < res.size; i++) {
        waitingFriends.append(res.friends[i]);
        newFriend(res.friends[i].personID,
                  res.friends[i].personName, res.friends[i].text);
    }

    slot_offlineMessage();
}


void Client::slot_waitingGroups()
{
    C2S::WaitingGroups msg;
    msg.userID = usrID;
    msg.type = C2S::MSG_WAITINNG_GROUP;
    time(&msg.sendTime);

    s->sendMessage((char *)&msg, sizeof(msg));
}


void Client::getWaitingGroups(S2C::NewJoinWaiting& res)
{
    for (int i = 0; i < res.size; i++) {
        waitingGroups.append(res.members[i]);
        newJoin(res.members[i].senderID, res.members[i].senderName,
                res.members[i].groupID, res.members[i].text);
    }

    slot_waitingFriends();
}

void Client::newFriend(int senderID, QString name, QString text)
{
    // 显示
    acceptReq* ac = new acceptReq();
    connect(ac, SIGNAL(signal_acceptReq(bool)), this, SLOT(slot_acceptReq(bool)));
    ac->setUi(0, senderID, name, text);
    ac->show(); 
}

void Client::newJoin(int senderID, QString name, int groupID, QString text)
{
    // 显示
    acceptReq* ac = new acceptReq();
    connect(ac, SIGNAL(signal_acceptReq(bool)), this, SLOT(slot_acceptReq(bool)));
    ac->setUi(groupID, senderID, name, text);
    ac->show();
}


void Client::newText(int groupID)
{
    // 已经初始化窗口，在现在的窗口中加载记录
    for(int i = 0; i < chat_w.length(); i++)
    {
        if(chat_w[i]->groupid == groupID)
        {
           QVector<Msg> temp_msgs = manager->getMsg(groupID);
           chat_w[i]->loadMessageHis(temp_msgs, usrID);
           return;
        }
    }

    // 在列表中标识新消息
    for(int i = 0; i < this->main_w->len; i++)
    {
        if(this->main_w->items[i].group_id == groupID)
        {
            this->main_w->items[i].setItemLoad();
            return;
        }
    }
}

void Client::slot_offlineMessage()
{
    C2S::Time msg;
    msg.userID = usrID;
    msg.type = C2S::MSG_TIME;
    time(&msg.sendTime);

    s->sendMessage((char *)&msg, sizeof(msg));
}

void Client::getOfflineMessage(S2C::Time& res)
{
    for (int i = 0; i < res.size; i++) {
        waitingText.append(res.group[i]);
        if (res.group[i].ifnew)
            newText(res.group[i].groupID);
    }
    init_flag = false;

}
