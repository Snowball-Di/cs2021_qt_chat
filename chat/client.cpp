#include "client.h"
#include <windows.h>

Client* Client::client = nullptr;

Client::Client(QObject *parent) : QObject(parent)
{
    // 启动UI
    /*
     * 待完成
     */

    s = Socket::getSocket();
}

Client* Client::client_init()
{
    if (client == nullptr)
        client = new Client();

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

    delete[] info.data;
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
        /*
         * 待完成
         */
    } else {
        // 登陆失败
        /*
         * 待完成
         */
    }

    qDebug() << "login success: " << res->success;
    qDebug() << "user's name：" << res->text;
    this->usrID = usrID;
    this->usrName = res->text;
    delete[] info.data;
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
    delete[] info.data;
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

    delete[] info.data;
}

void Client::slot_acceptReq(int targetID, bool accept, bool isFriend)
{
    C2S::Accept msg;
    msg.senderID = targetID;
    msg.targetID = usrID;
    msg.type = C2S::MSG_ACCEPT;
    msg.kind = isFriend;
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
        // 发送成功
        slot_friendList();
        /*
         * 待完成
         */
    } else {
        // 发送失败
        /*
         * 待完成
         */
    }

    delete[] info.data;
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
        // 创建成功
        slot_groupList();
        /*
         * 待完成
         */
    } else {
        // 创建失败
        /*
         * 待完成
         */
    }

    delete[] info.data;
}


void Client::slot_friendList()
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

    // UI更新好友列表
    /*
     * 待完成
     */

    delete[] info.data;
}

void Client::slot_groupList()
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

    // UI更新群组列表
    /*
     * 待完成
     */

    delete[] info.data;
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

    delete[] info.data;
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
        SocketMsg m = s->nextPendingMessage();
        if (m.type != 0) {
            switch (m.type) {
            case S2C::SERVER_NEWFRIEND :
                break;
            case S2C::SERVER_NEWGROUP:
                break;
            case S2C::SERVER_NEWJOIN:
                break;
            case S2C::SERVER_MSG_TEXT:

            }
        }
    }
}
