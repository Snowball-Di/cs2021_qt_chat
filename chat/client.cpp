#include "client.h"

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

    SocketMsg info = {S2C::SERVER_REPLY_LOGIN, 0};

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
    qDebug() << "登录回复：" << res->text;
    this->usrID = usrID;
}

void Client::sendText(int groupID)
{
    C2S::Text msg;
    msg.senderID = usrID;
    msg.type = C2S::MSG_LOG;
    time(&msg.sendTime);
    msg.operation = true;

    memcpy(msg.password, password.data(), password.length());
    msg.password[password.length()] = 0;

    s->sendMessage((char *)&msg, sizeof(msg));

    SocketMsg info = {S2C::SERVER_REPLY_LOGIN, 0};

    if (!waiting(info)) {
        qDebug() << "fail to login.";
        return;
    }

    S2C::Response* res = (S2C::Response*)info.data;
    // 显示注册信息
    /*
     * 待完成
     */
    qDebug() << "login success: " << res->success;
    qDebug() << "登录回复：" << res->text;
}

void Client::sendFriendRequest(int friendID){}
void Client::acceptFriendRequest(){}

void Client::newGroup(QString groupName){}
void Client::joinGroup(int groupID){}
void Client::acceptJoinGroup(int groupID){}

bool Client::waiting(SocketMsg& msg)
{
    time_t begin, now;
    time(&begin);

    while (now - begin <= 7) {
        auto m = s->getResponse();
        if (m.type == msg.type) {
            msg = m;
            return true;
        }
        time(&now);
    }
    return false;
}
