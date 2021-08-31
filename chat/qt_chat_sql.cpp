#include "qt_chat_sql.h"

Qt_chat_sql* Qt_chat_sql::Sqlhand=nullptr;
int Qt_chat_sql::usrID=1000;
int Qt_chat_sql::grpID=10000;
Qt_chat_sql::Qt_chat_sql(QWidget *parent) : QWidget(parent)
{

}

bool Qt_chat_sql::InitDatabase(){//测试完成
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(QApplication::applicationDirPath()+"/chat.db");
    }
    if(!database.open()){
        qDebug()<<"Cannot open database !";
        return false;
    }
    else{

        sqlquery = QSqlQuery(database);
        sqlquery.exec("CREATE TABLE friendRequest (senderID INT,recieverID INT,content VARCHAR,PRIMARY KEY(senderID,recieverID),FOREIGN KEY(senderID) REFERENCES user_info (userID),FOREIGN KEY (recieverID) REFERENCES user_info (userID));");
        sqlquery.exec("CREATE TABLE user_info (userID INT PRIMARY KEY,userName VARCHAR,password VARCHAR,sculptureaddress VARCHAR);");
        sqlquery.exec("CREATE TABLE group_info (groupID INT PRIMARY KEY,groupName VARCHAR,type INT,friendID1 INT,friendID2 INT,lasttime DATETIME,FOREIGN KEY (friendID1) REFERENCES user_info (userID),FOREIGN KEY (friendID2) REFERENCES user_info (userID));");
        sqlquery.exec("CREATE TABLE groupRequest (senderID INT,groupID INT,content VARCHAR,recieverID INT,PRIMARY KEY(groupID,senderID),FOREIGN KEY (groupID) REFERENCES group_info (groupID),FOREIGN KEY (senderID) REFERENCES user_info (userID),FOREIGN KEY (recieverID) REFERENCES user_info (userID));");

        return true;
    }
}



int Qt_chat_sql::Register(QString log_name, QString password){//测试完成
    usrID++;
    bool ok1,ok2;
    QString userID=QString::number(usrID);

    sqlquery = QSqlQuery(database);
    //开始在用户信息表中添加新用户
    sqlquery.prepare("INSERT INTO user_info (userID,userName,password,sculptureaddress) VALUES (:usrID,:lgname,:psw,NULL)");
    sqlquery.bindValue(":usrID",userID);
    sqlquery.bindValue(":lgname",log_name);
    sqlquery.bindValue(":psw",password);

    ok1=sqlquery.exec();//标记是否成功


    //建立该用户的好友列表TABLE
    QString userList="List_";
    userList.append(userID);

    QString creatlist="CREATE TABLE ";
    creatlist.append(userList);
    creatlist.append(" (groupID INT,friendID INT,PRIMARY KEY (groupID),FOREIGN KEY (groupID) REFERENCES group_info (groupID),FOREIGN KEY (friendID) REFERENCES user_info (userID));");

    ok2=sqlquery.exec(creatlist);

    if(ok1&&ok2){
        return 0;
    }

    return usrID;

}

int Qt_chat_sql::newGroup(bool kind,QString groupName,int usrID){//新建群组，有好友也有群组   测试完成   更改lasttime
    grpID++;
    QString groupID=QString::number(grpID);
    QString userID=QString::number(usrID);

    if(kind==0){
        //该建群申请为好友申请，无需建立群成员TABLE    测试完成

        bool ok1,ok2,ok3,ok4;


        //添加群组信息   测试完成
        sqlquery.prepare("INSERT INTO group_info (groupID,groupName,type,friendID1,friendID2,lasttime) VALUES (:groupID,:groupName,:type,:friendID1,:friendID2,0)");
        sqlquery.bindValue(":groupID",groupID);
        sqlquery.bindValue(":groupName","NULL");
        sqlquery.bindValue(":type","0");
        sqlquery.bindValue(":friendID1",userID);
        sqlquery.bindValue(":friendID2",groupName);

        ok1=sqlquery.exec();

        //建立该群组的消息记录TABLE
        QString groupMsg="Msg_";
        groupMsg.append(groupID);

        QString createMsg="CREATE TABLE ";
        createMsg.append(groupMsg);
        createMsg.append(" (time DATETIME,senderID INT,type VARCHAR,content VARCHAR,PRIMARY KEY (time,senderID),FOREIGN KEY (senderID) REFERENCES user_info (userID))");
        ok2=sqlquery.exec(createMsg);


        //在用户1的好友列表中添加群聊
        QString userfriend="List_";
        userfriend.append(userID);
        sqlquery.prepare("INSERT INTO "+userfriend+" (groupID,friendID) VALUES (:groupID,:friendID)");
        sqlquery.bindValue(":groupID",groupID);
        sqlquery.bindValue(":friendID",groupName);

        ok3=sqlquery.exec();

        //在用户2的好友列表中添加群聊
        QString senderfriend="List_";
        senderfriend.append(groupName);
        sqlquery.prepare("INSERT INTO "+senderfriend+" (groupID,friendID) VALUES (:groupID,:friendID)");
        sqlquery.bindValue(":groupID",groupID);
        sqlquery.bindValue(":friendID",userID);

        ok4=sqlquery.exec();

        if(ok1&&ok2&&ok3&&ok4){
            return grpID;
        }
        else{
            return 0;
        }

    }
    else{
        //该建群申请为建群申请，下需添加群信息TABLE，建立群成员TABLE，添加user的好友列表，建立群消息记录TABLE


        //建立群成员TABLE  测试完成
        QString groupMem="Mem_";
        groupMem.append(groupID);

        sqlquery.prepare("CREATE TABLE "+groupMem+" (memberID INT,PRIMARY KEY (memberID),FOREIGN KEY (memberID) REFERENCES user_info (userID))");

        bool ok1=sqlquery.exec();

        //在群成员列表中添加申请人user  测试完成
        sqlquery.prepare("INSERT INTO "+groupMem+" (memberID) VALUES (:memberID)");
        sqlquery.bindValue(":memberID",userID);

        bool ok2=sqlquery.exec();

        //添加群信息   测试完成
        sqlquery.prepare("INSERT INTO group_info (groupID,groupName,type,friendID1,friendID2,lasttime) VALUES (:groupID,:groupName,:type,:friendID1,:friendID2,0)");
        sqlquery.bindValue(":groupID",groupID);
        sqlquery.bindValue(":groupName",groupName);
        sqlquery.bindValue(":type","1");
        sqlquery.bindValue(":friendID1",userID);
        sqlquery.bindValue(":friendID2","NULL");

        bool ok3=sqlquery.exec();

        //添加好友列表
        QString userfriend="List_";
        userfriend.append(userID);

        sqlquery.prepare("INSERT INTO "+userfriend+" (groupID,friendID) VALUES (:groupID,:friendID)");
        sqlquery.bindValue(":groupID",groupID);
        sqlquery.bindValue(":friendID","NULL");

        bool ok4=sqlquery.exec();

        //建立消息记录TABLE
        QString groupMsg="Msg_";
        groupMsg.append(groupID);

        sqlquery.prepare("CREATE TABLE "+groupMsg+" (time DATETIME,senderID INT,type VARCHAR,content VARCHAR,PRIMARY KEY (time,senderID),FOREIGN KEY (senderID) REFERENCES user_info (userID))");
        bool ok5=sqlquery.exec();

        if(ok1&&ok2&&ok3&&ok4&&ok5){
            return grpID;
        }
        else{
            return 0;
        }
    }

}

bool Qt_chat_sql::addMember(int grpID, int usrID){//测试完成
    QString userID=QString::number(usrID);
    QString groupID=QString::number(grpID);

    //该接口需要添加群组成员列表，添加好友列表

    //添加群组成员列表
    QString groupMem="Mem_";
    groupMem.append(groupID);

    sqlquery.prepare("INSERT INTO "+groupMem+" (memberID) VALUES (:memberID)");
    sqlquery.bindValue(":memberID",userID);

    bool ok1=sqlquery.exec();

    //添加好友列表
    QString userfriend="List_";
    userfriend.append(userID);
    sqlquery.prepare("INSERT INTO "+userfriend+" (groupID,friendID) VALUES (:groupID,NULL)");
    sqlquery.bindValue(":groupID",groupID);

    bool ok2=sqlquery.exec();

    if(ok1&&ok2){
        return true;
    }
    else{
        return false;
    }

}

bool Qt_chat_sql::deleteGroup(int grpID, int usrID){//测试完成
    //该接口需要 1.查询申请人权限 2.有权限，则删除群组信息 3.删除群组成员好友列表 4.删除群组成员TABLE
    int type;
    int hostID;
    QString groupID=QString::number(grpID);
    QString userID=QString::number(usrID);

    bool ok[100];
    int i=0;

    //查询申请人是否为群主
    sqlquery.prepare("SELECT * FROM group_info WHERE groupID = :grpID");
    sqlquery.bindValue(":grpID",groupID);

    sqlquery.exec();

    sqlquery.first();
    type=sqlquery.value(2).toInt();
    hostID=sqlquery.value(3).toInt();

    if(type == 1 && hostID == usrID){
        //申请人权限足够

        //删除群组信息  测试完成
        sqlquery.prepare("DELETE FROM group_info WHERE groupID = :grpID");
        sqlquery.bindValue(":grpID",groupID);

        bool ok1=sqlquery.exec();

        //删除群组成员的对应好友列表
        int memberID[100]={0};
        QString groupMem="Mem_";
        QString userList;
        groupMem.append(groupID);

            //获取群组成员列表
        sqlquery.prepare("SELECT * FROM "+groupMem);

        sqlquery.exec();
        while(sqlquery.next()){
            memberID[i]=sqlquery.value(0).toInt();
            ++i;
        }

            //开始删除群组成员对应的好友列表
        for(int j=0;j<i;++j){
            userList="List_";
            userList.append(QString::number(memberID[j]));

            sqlquery.prepare("DELETE FROM "+userList+" WHERE groupID = :grpID");
            sqlquery.bindValue(":grpID",groupID);

            ok[j]=sqlquery.exec();
        }

        //删除群组成员TABLE
        sqlquery.prepare("DROP TABLE "+groupMem);

        bool ok2=sqlquery.exec();
        if(ok1&&ok2){
            for(int k=0;k<i;++k){
                if(ok[k]==0){
                    return false;
                }
            }
            return true;
        }
        else{
            return false;
        }
    }
    else{
        //申请人权限不足，无法解散群组
        return false;
    }
}

int Qt_chat_sql::getFriendList(int usrID, FRIEND_LIST* friendlist){//测试完成
    QString userID=QString::number(usrID);
    QString userList="List_";
    userList.append(userID);
    int i=0;
    //获得对应好友列表TABLE 中的信息
    sqlquery.prepare("SELECT * FROM "+userList);

    bool ok=sqlquery.exec();
    if(ok==0){
        return -1;
    }

    while(sqlquery.next()){
        friendlist[i].groupID=sqlquery.value(0).toInt();
        friendlist[i].friendID=sqlquery.value(1).toInt();
        ++i;
    }

    return i;

}

int Qt_chat_sql::groupMembers(int grpID, int *group_members){//测试完成
    QString groupID=QString::number(grpID);
    QString groupMem="Mem_";
    groupMem.append(groupID);
    int i=0;

    //查询grpID_Mem TABLE 中的成员信息
    sqlquery.prepare("SELECT * FROM "+groupMem);

    bool ok=sqlquery.exec();
    if(ok==0){
        return false;
    }

    while(sqlquery.next()||i>=100){
        group_members[i]=sqlquery.value(0).toInt();
        ++i;
    }

    return i;
}

bool Qt_chat_sql::login(int usrID, QString password, USER_INFO &user_info){//测试完成
    QString userID=QString::number(usrID);
    QString firmword;

    //该接口为登录接口，确认登录用户名及密码正确后，将user_info写入

    sqlquery.prepare("SELECT * FROM user_info WHERE userID = "+userID);

    //查询语句是否执行成功
    bool ok=sqlquery.exec();
    qDebug()<<ok;
    if(ok==0){
        return false;
    }

    //用户是否存在
    bool firm=sqlquery.first();
    if(firm==0){
        return false;
    }

    //密码是否正确
    firmword=sqlquery.value(2).toString();
    if(firmword==password){

        user_info.userID=usrID;
        user_info.address=sqlquery.value(3).toString();
        user_info.password=firmword;
        user_info.userName=sqlquery.value(1).toString();

        return true;
    }

    else{
        return false;
    }

}

bool Qt_chat_sql::editUser_info(int usrID, USER_INFO user_info){//测试完成
    QString userID=QString::number(usrID);

    //该接口更改用户信息


    sqlquery.prepare("UPDATE user_info SET userName = '"+user_info.userName+"' , password = '"+user_info.password+"' , sculptureaddress = '"+user_info.address+"' WHERE userID = "+userID);

    bool ok=sqlquery.exec();
    if(ok==0){
        return false;
    }
    else{
        return true;
    }
}

bool Qt_chat_sql::deleteMember(int grpID, int usrID, int sdrID,bool type){//0:删除好友 1:将别人踢出群组  userID为被踢或删的用户
    QString groupID=QString::number(grpID);
    QString userID=QString::number(usrID);
    QString senderID=QString::number(sdrID);

    //该接口为删除好友或踢出群组的接口

    if(type==0){//删除好友

        //删除好友，需要删除群组信息，在两个人的好友列表中删除好友，删除群组消息记录TABLE

        database.transaction();//事务开始


        //删除群组信息
        sqlquery.prepare("DELETE FROM group_info WHERE groupID = :grpID");
        sqlquery.bindValue(":grpID",groupID);

        bool ok1=sqlquery.exec();


        //在sender好友列表中删除好友
        QString senderList=senderID;
        senderList.append("_List");

        sqlquery.prepare("DELETE FROM :usrID_List WHERE groupID = :grpID");
        sqlquery.bindValue(":usrID_List",senderList);
        sqlquery.bindValue(":grpID",groupID);

        bool ok2=sqlquery.exec();


        //在user好友列表中删除好友
        QString userList=userID;
        userList.append("_List");

        sqlquery.prepare("DELETE FROM :usrID_List WHERE groupID = :grpID");
        sqlquery.bindValue(":usrID_List",userList);
        sqlquery.bindValue(":grpID",groupID);

        bool ok3=sqlquery.exec();


        //删除群组的消息记录TABLE
        QString groupMsg=groupID;
        groupMsg.append("_Msg");

        sqlquery.prepare("DELETE TABLE :grpMsg");
        sqlquery.bindValue(":grpMsg",groupMsg);

        bool ok4=sqlquery.exec();

        if(ok1&&ok2&&ok3&&ok4){
            //均执行成功，提交操作
            database.commit();
            return true;
        }
        else{
            //执行失败，回滚操作，返回0
            database.rollback();
            return false;
        }
    }
    else{//踢出群组

        int type;
        QString hostID;
        //踢出群组，先确认sender的身份是否为群主，如果是，则需要在成员表中删除该成员，并在user的好友列表中删除该群组

        database.transaction();//事务开始

        //确认sender身份
        sqlquery.prepare("SELECT * FROM group_info WHERE groupID = :grpID");
        sqlquery.bindValue(":grpID",groupID);

        sqlquery.exec();

        sqlquery.first();
        type=sqlquery.value(2).toInt();
        hostID=sqlquery.value(3).toString();

        if(type == 1 && hostID == senderID){

            //权限足够，在成员表中删除从成员
            QString groupMem=groupID;
            groupMem.append("_Mem");

            sqlquery.prepare("DELETE FROM :grpMem WHERE memberID = :usrID");
            sqlquery.bindValue(":grpMem",groupMem);
            sqlquery.bindValue(":usrID",userID);

            bool ok1=sqlquery.exec();


            //在user好友列表中删除群
            QString userList=userID;
            userID.append("_List");

            sqlquery.prepare("DELETE FROM :usrList WHERE groupID = :grpID");
            sqlquery.bindValue(":usrList",userList);
            sqlquery.bindValue(":grpID",groupID);

            bool ok2=sqlquery.exec();

            if(ok1&&ok2){
                //均执行成功，提交操作
                database.commit();
                return true;
            }
            else{
                //执行失败，回滚操作，返回0
                database.rollback();
                return false;
            }
        }
        else{
            //权限不足，踢出失败
            return false;
        }
    }
}

bool Qt_chat_sql::getUser_info(QStringList userIDlist, USER_INFO *user_infoList){//测试完成
    QString userID;
    bool ok[100];
    int i=0;

    for(i=0;i<userIDlist.count()||i>=100;++i){
        userID=userIDlist.value(i);

        sqlquery.prepare("SELECT * FROM user_info WHERE userID = "+userID);
        sqlquery.bindValue(":usrID",userID);

        ok[i]=sqlquery.exec();

        sqlquery.first();
        user_infoList[i].userID=sqlquery.value(0).toInt();
        user_infoList[i].userName=sqlquery.value(1).toString();
        user_infoList[i].password=sqlquery.value(2).toString();
        user_infoList[i].address=sqlquery.value(3).toString();
    }
    for(int j=0;j<=i;++j){
        if(ok[j]==0){
            return false;
        }
    }
    return true;
}

bool Qt_chat_sql::newMsg(MESSAGE msg){//写入时间可能有问题
    QString groupID=QString::number(msg.groupID);
    QString senderID=QString::number(msg.senderID);
    QDateTime time=msg.time;

    //插入消息记录表，更新群组信息最新时间记录

    //插入消息记录表
    QString groupMsg="Msg_";
    groupMsg.append(groupID);


    sqlquery.prepare("INSERT INTO "+groupMsg+" (time,senderID,type,content) VALUES (:tm,:sdrID,:tp,:ctt)");
    sqlquery.bindValue(":tm",time);
    sqlquery.bindValue(":sdrID",senderID);
    sqlquery.bindValue(":tp",msg.type);
    sqlquery.bindValue(":ctt",msg.content);

    bool ok1=sqlquery.exec();



    //更新群组信息表
    sqlquery.prepare("UPDATE group_info SET lasttime = :tm");
    sqlquery.bindValue(":tm",time);

    bool ok2=sqlquery.exec();


    if(ok1&&ok2){
        return true;
    }
    else{
        return false;
    }
}

int Qt_chat_sql::get_history(int limit, int grpID, MESSAGE *messagelist,QDateTime time){//时间可能有问题
    QString selectMsg;
    QString groupID=QString::number(grpID);
    QString groupMsg="Msg_";
    groupMsg.append(groupID);
    int sum=0;

    sqlquery.prepare("SELECT * FROM "+groupMsg+" ORDER BY time DESC");

    bool ok=sqlquery.exec();

    if(ok==0){
        return 0;
    }

    while(sqlquery.next()||sum>limit){

        if(sqlquery.value(0).toDateTime()>time){
            sum++;
            messagelist[sum].time=sqlquery.value(0).toDateTime();
            messagelist[sum].senderID=sqlquery.value(1).toInt();
            messagelist[sum].type=sqlquery.value(2).toString();
            messagelist[sum].content=sqlquery.value(3).toString();
        }
    }

    return sum;
}

int Qt_chat_sql::getGroup_info(QStringList groupIDList, GROUP_INFO* groupinfo){
    QString grpName;
    bool ok;
    int i;
    for(i=0;i<groupIDList.count();++i){
        sqlquery.prepare("SELECT * FROM group_info WHERE groupID = :grpID");
        sqlquery.bindValue(":grpID",groupIDList.value(i));

        ok=sqlquery.exec();
        if(ok==0){
            return false;
        }

        sqlquery.first();
        groupinfo[i].time=sqlquery.value(5).toDateTime();
        groupinfo[i].type=sqlquery.value(2).toInt();
        groupinfo[i].groupID=sqlquery.value(0).toInt();
        groupinfo[i].masterID=sqlquery.value(3).toInt();
        groupinfo[i].groupName=sqlquery.value(1).toString();
        groupinfo[i].friendID=sqlquery.value(4).toInt();
    }
    return i;
}

bool Qt_chat_sql::addGrouprequest(int sdrID, int grpID,QString content){
    QString groupID=QString::number(grpID);
    QString senderID=QString::number(sdrID);
    int masterID;

    sqlquery.prepare("SELECT * FROM group_info WHERE groupID = :grpID");
    sqlquery.bindValue(":grpID",groupID);
    bool ok1=sqlquery.exec();

    if(ok1==0){
        return false;
    }
    sqlquery.first();
    masterID=sqlquery.value(3).toInt();

    sqlquery.prepare("INSERT INTO groupRequest (senderID,groupID,content,recieverID) VALUES (:sdrID,:grpID,:content,:recID)");
    sqlquery.bindValue(":sdrID",senderID);
    sqlquery.bindValue(":grpID",groupID);
    sqlquery.bindValue(":content",content);
    sqlquery.bindValue(":recID",masterID);

    bool ok2=sqlquery.exec();

    if(ok2){
        return true;
    }
    else{
        return false;
    }
}

int Qt_chat_sql::getGrouprequest(int usrID, GROUP_REQUEST* grprequest){
    QString userID=QString::number(usrID);
    int i=0;

    sqlquery.prepare("SELECT * FROM groupRequest WHERE recieverID = :usrID");
    sqlquery.bindValue(":usrID",userID);

    bool ok=sqlquery.exec();

    if(ok==0){
        return -1;
    }

    while(sqlquery.next()){
        grprequest[i].senderID=sqlquery.value(0).toInt();
        grprequest[i].groupID=sqlquery.value(1).toInt();
        grprequest[i].content=sqlquery.value(2).toString();
        ++i;
    }

    return i;
}

bool Qt_chat_sql::deleteGrouprequest(int sdrID, int grpID){
    QString senderID=QString::number(sdrID);
    QString groupID=QString::number(grpID);

    sqlquery.prepare("DELETE FROM groupRequest WHERE senderID = :sdrID AND groupID = :grpID");
    sqlquery.bindValue(":sdrID",senderID);
    sqlquery.bindValue(":grpID",groupID);

    bool ok=sqlquery.exec();

    if(ok){
        return true;
    }
    else{
        return false;
    }
}

bool Qt_chat_sql::addFriendrequest(int sdrID, int rcvID, QString content){
    QString senderID=QString::number(sdrID);
    QString recieverID=QString::number(rcvID);

    sqlquery.prepare("INSERT INTO friendRequest (senderID,recieverID,content) VALUES (:sdrID,:rcvID,:content)");
    sqlquery.bindValue(":sdrID",senderID);
    sqlquery.bindValue(":rcvID",recieverID);
    sqlquery.bindValue(":content",content);

    bool ok=sqlquery.exec();

    if(ok){
        return true;
    }
    else{
        return false;
    }
}

int Qt_chat_sql::getFriendrequest(int usrID, FRIEND_REQUEST *frdrequest){
    QString userID=QString::number(usrID);
    int i=0;

    sqlquery.prepare("SELECT * FROM friendRequest WHERE recieverID = :usrID");
    sqlquery.bindValue(":usrID",userID);

    bool ok=sqlquery.exec();

    if(ok==0){
        return -1;
    }

    while(sqlquery.next()){
        frdrequest[i].senderID=sqlquery.value(0).toInt();
        frdrequest[i].recieverID=sqlquery.value(1).toInt();
        frdrequest[i].content=sqlquery.value(2).toString();
        ++i;
    }

    return i;
}

bool Qt_chat_sql::deleteFriendrequest(int sdrID, int rcvID){
    QString senderID=QString::number(sdrID);
    QString recieverID=QString::number(rcvID);

    sqlquery.prepare("DELETE FROM friendRequest WHERE senderID = :sdrID AND recieverID = :rcvID");
    sqlquery.bindValue(":sdrID",senderID);
    sqlquery.bindValue(":grpID",recieverID);

    bool ok=sqlquery.exec();

    if(ok){
        return true;
    }
    else{
        return false;
    }
}

int Qt_chat_sql::getgroupOwner(int grpID){
    QString groupID=QString::number(grpID);
    int ownerID;

    sqlquery.prepare("SELECT * FROM group_info WHERE groupID = :grpID");
    sqlquery.bindValue(":grpID",groupID);

    bool ok=sqlquery.exec();

    if(ok==0){
        return false;
    }

    sqlquery.first();
    ownerID=sqlquery.value(3).toInt();

    return ownerID;
}

QString Qt_chat_sql::getUserName(int usrID){
    QString userID=QString::number(usrID);

    sqlquery.prepare("SELECT * FROM user_info WHERE userID = :usrID");
    sqlquery.bindValue(":usrID",userID);

    bool ok=sqlquery.exec();

    if(ok==0){
        return 0;
    }

    sqlquery.first();
    return sqlquery.value(1).toString();
}

QString Qt_chat_sql::getGroupName(int grpID){
    QString groupID=QString::number(grpID);

    sqlquery.prepare("SELECT * FROM group_info WHERE groupID = :grpID");
    sqlquery.bindValue(":grpID",groupID);

    bool ok=sqlquery.exec();

    if(ok==0){
        return 0;
    }

    sqlquery.first();
    return sqlquery.value(1).toString();
}
