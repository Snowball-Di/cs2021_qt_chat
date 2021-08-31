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

void Client::userRegister(QString name, QString password)
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
    // 显示注册信息
    /*
     * 待完成
     */

    qDebug() << "register success.";
    qDebug() << "注册ID：" << res->text;

    delete[] info.data;
}

void Client::login(int usrID, QString password)
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
    // 登录操作
    /*
     * 待完成
     */
    qDebug() << "login success: " << res->success;
    qDebug() << "user's name：" << res->text;
    this->usrID = usrID;
    this->usrName = res->text;
    delete[] info.data;
}

void Client::sendText(int groupID, QString text)
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
    // UI显示
    /*
     * 待完成
     */
    qDebug() << QString("%1 : ").arg(usrID) + res->text;
    delete[] info.data;
}

void Client::friendReq(int friendID, QString verifyText)
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
    // UI显示
    /*
     * 待完成
     */
    qDebug() << QString("%1 : ").arg(usrID) + res->text;
    delete[] info.data;
}

void Client::acceptFriendRequest()
{

}

void Client::newGroup(QString groupName)
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
        qDebug() << "fail to send request.";
        return;
    }

    S2C::Response* res = (S2C::Response*)info.data;
    // UI显示
    /*
     * 待完成
     */
    qDebug() << QString("%1 : ").arg(usrID) + res->text;
    delete[] info.data;
}

void Client::joinGroup(int groupID)
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
    // UI显示
    /*
     * 待完成
     */
    qDebug() << QString("%1 : ").arg(usrID) + res->text;
    delete[] info.data;
}

void Client::acceptJoinGroup(int groupID)
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
    // UI显示
    /*
     * 待完成
     */
    qDebug() << QString("%1 : ").arg(usrID) + res->text;
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
