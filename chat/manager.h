#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QFile>
#include <QDir>

// 用于存放好友列表
struct Friend
{
    int id;
    int group_id;
    QString name;
    QString avatar_path;
};

// 用于存放群组列表
struct Group
{
    int group_id;
    QString name;
};

// 用于存放消息
struct Msg
{
    int senderID;
    QString senderName;
    time_t sendTime;
    QString text;
};


/*
 * 使用完后务必析构！！！！
 */
class Manager
{
public:
    // 获取上次登录的用户id
    // 返回0表示上次未保存id
    static int getLastID();

    // 获取文件管理者单例，登录成功后调用
    // _save_account 表示是否保存ID
    static Manager* getManager(const int usrID, bool _save_account);
    bool isBusy() {return busy;};

    QVector<Friend> getFriends();
    QVector<Group> getGroups();
    void setFriends(QVector<Friend> &friends);
    void setGroups(QVector<Group> &groups);

    QVector<Msg> getMsg(int groupID);
    void setMsg(int groupID, QVector<Msg>& messages);

    QString getFriendName(int id);

private:
    ~Manager();
    explicit Manager(QString id);
    static Manager* m;
    int id;
    QString root;
    QFile usr_name;
    QFile friends_list;
    QFile groups_list;

    bool save_account;
    QString last_usr;

    bool busy = false;
};




/*
 * 本地文件：
 * 好友列表
 * 消息记录
 * 昵称
 * 头像
 *
 * ./{usrID}
 * ./{usrID}/avatar.png
 * ./{usrID}/usrname.txt
 * ./{usrID}/record/{groupID}
 * ./{usrID}/friends.list segment: avatar, name, id, groupid
 * ./{usrID}/groups.list segment: avatar, name, id
 * records/lastusr +
 * 返回最后一条消息记录
 */

#endif // MANAGER_H
