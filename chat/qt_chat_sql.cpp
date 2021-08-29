#include "qt_chat_sql.h"

Qt_chat_sql* Qt_chat_sql::Sqlhand=nullptr;
int Qt_chat_sql::usrID=1000;
int Qt_chat_sql::grpID=10001;
Qt_chat_sql::Qt_chat_sql(QWidget *parent) : QWidget(parent)
{

}

bool Qt_chat_sql::InitDatabase(){
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("chat.db");
    }
    if(!database.open()){
        qDebug()<<"Cannot open database !";
        return false;
    }
    else{
        qDebug() << "Database open successfully !";
        sqlquery = QSqlQuery(database);
        sqlquery.exec("CREATE TABLE friendList (userID INT,groupID INT,friendID INT,PRIMARY KEY(userID,groupID));");
        sqlquery.exec("CREATE TABLE user_info (userID INT PRIMARY KEY,userName VARCHAR,password VARCHAR,sculptureaddress VARCHAR);");
        sqlquery.exec("CREATE TABLE group_info (groupID INT PRIMARY KEY,groupName VARCHAR,type INT,friendID1 INT,friendID2 INT,FOREIGN KEY (friendID1) REFERENCES user_info (userID),FOREIGN KEY (friendID2) REFERENCES user_info (userID));");
        sqlquery.exec("CREATE TABLE groupMember (groupID INT,memberID INT,PRIMARY KEY(groupID,memberID),FOREIGN KEY (groupID) REFERENCES group_info (groupID),FOREIGN KEY (memberID) REFERENCES user_info (userID));");
        sqlquery.exec("CREATE TABLE Message (groupID INT,time INT,senderID INT,type VARCHAR,content VARCHAR,PRIMARY KEY(groupID,senderID,time),FOREIGN KEY (groupID) REFERENCES group_info (groupID),FOREIGN KEY (senderID) REFERENCES user_info (userID));");
        qDebug()<<database.tables().count();
        return true;
    }
}



int Qt_chat_sql::Register(QString log_name, QString password){//插入失败功能未添加
    QString insertTable;
    QString userID=QString::number(usrID);
    insertTable="INSERT INTO user_info (userID,userName,password,sculptureaddress) VALUES (";

    insertTable.append(userID);
    insertTable.append(",'");
    insertTable.append(log_name);
    insertTable.append("','");
    insertTable.append(password);
    insertTable.append("',NULL);");

    sqlquery.exec(insertTable);

//    sqlquery.exec("select * from user_info");
//    while(sqlquery.next()){
//        qDebug()<<sqlquery.value(0).toInt()
//                <<sqlquery.value(1).toString()
//                <<sqlquery.value(2).toString()
//                <<sqlquery.value(3).toString();

//    }
    usrID++;
    return usrID;

}

int Qt_chat_sql::newGroup(bool kind,QString groupName,int usrID){//插入失败功能未添加
    QString insertTable;
    QString insertFriend;
    QString insertFriend2;
    QString groupID=QString::number(grpID);
    QString userID=QString::number(usrID);
    insertTable="INSERT INTO group_info (groupID,groupName,type,friendID1,friendID2) VALUES (";

    if(kind==0){//还应该添加ID1和ID2互换的元组
        insertTable.append(groupID);
        insertTable.append(",NULL,0,");
        insertTable.append(userID);
        insertTable.append(",");
        insertTable.append(groupName);
        insertTable.append(")");

        //在好友列表中添加好友
        insertFriend="INSERT INTO friendList (userID,groupID,friendID) VALUES (";
        insertFriend.append(userID);
        insertFriend.append(",");
        insertFriend.append(groupID);
        insertFriend.append(",");
        insertFriend.append(groupName);
        insertFriend.append(")");


        insertFriend2="INSERT INTO friendList (userID,groupID,friendID) VALUES (";
        insertFriend.append(groupName);
        insertFriend.append(",");
        insertFriend.append(groupID);
        insertFriend.append(",");
        insertFriend.append(userID);
        insertFriend.append(")");

        sqlquery.exec(insertFriend);
        sqlquery.exec(insertFriend2);

    }
    else{
        insertTable.append(groupID);
        insertTable.append(",");
        insertTable.append(groupName);
        insertTable.append(",1,");
        insertTable.append(userID);
        insertTable.append(",NULL,)");

        /*
        在群组成员表中添加成员
        */

        QString insertgm;
        insertgm="INSERT INTO groupMember (groupID,memberID) VALUES (";
        insertgm.append(groupID);
        insertgm.append(",");
        insertgm.append(userID);
        insertgm.append(");");
        sqlquery.exec(insertgm);

        //在好友列表中添加群组
        QString insertList;

        insertList="INSERT INTO friendList (userID,groupID,friendID) VALUES (";
        insertList.append(userID);
        insertList.append(",");
        insertList.append(groupID);
        insertList.append(",NULL)");

        sqlquery.exec(insertList);
    }


    sqlquery.exec(insertTable);


//    sqlquery.exec("select * from user_info");
//    while(sqlquery.next()){
//        qDebug()<<sqlquery.value(0).toInt()
//                <<sqlquery.value(1).toString()
//                <<sqlquery.value(2).toString()
//                <<sqlquery.value(3).toString();

//    }
    grpID++;
    return grpID;
}

bool Qt_chat_sql::addMember(int groupID, int userID){//插入失败功能未添加
    QString insertgm;
    QString insertfl;

    QString usrID=QString::number(userID);
    QString grpID=QString::number(groupID);

    insertgm="INSERT INTO groupMember (groupID,memberID) VALUES (";
    insertgm.append(grpID);
    insertgm.append(",");
    insertgm.append(usrID);
    insertgm.append(");");

    insertfl="INSERT INTO friendList (userID,groupID,friendID) VALUES (";
    insertfl.append(usrID);
    insertfl.append(",");
    insertfl.append(grpID);
    insertfl.append(",NULL)");


    sqlquery.exec(insertfl);
    sqlquery.exec(insertgm);
    return true;
}

bool Qt_chat_sql::deleteGroup(int groupID, int userID){
    /*
    查询申请人是否为群主
    */
    int type;
    int hostID;
    QString selectRight="SELECT * FROM group_info WHERE groupID = ";
    QString grpID=QString::number(groupID);
    selectRight.append(grpID);
    if(sqlquery.prepare(selectRight)){
        if(!sqlquery.exec(selectRight))
        {
            qDebug()<<sqlquery.lastError();
            return false;
        }
        else
        {
            while(sqlquery.next())
            {
                type=sqlquery.value(2).toInt();
                hostID=sqlquery.value(3).toInt();
            }
            if(type == 1 && hostID == userID){

                //确定申请人权限符合，开始删除群组信息

                QString deleteTable="DELETE FROM group_info WHERE groupID = ";
                deleteTable.append(grpID);
                if(sqlquery.prepare(deleteTable)){
                    if(!sqlquery.exec(deleteTable))
                    {
                        qDebug()<<sqlquery.lastError();
                    }
                    return false;
                }
                else{
                    //删除群组成员信息
                    QString deletemember="DELETE FROM groupMember WHERE groupID = ";
                    deletemember.append(grpID);
                    if(sqlquery.prepare(deletemember)){
                        if(!sqlquery.exec(deletemember))
                        {
                            qDebug()<<sqlquery.lastError();
                        }
                        return false;
                    }
                    else{
                        QString deletefl="DELETE FROM friendList WHERE groupID = ";
                        deletefl.append(grpID);
                        if(sqlquery.prepare(deletefl)){
                            if(!sqlquery.exec(deletefl))
                            {
                                qDebug()<<sqlquery.lastError();
                            }
                            return false;
                        }
                        else{

                            return true;
                        }
                    }
                }
            }
            else{
                if(type==0){
                    //删除好友
                    QString deletemember="DELETE FROM group_info WHERE groupID = ";
                    deletemember.append(grpID);
                    if(sqlquery.prepare(deletemember)){
                        if(!sqlquery.exec(deletemember))
                        {
                            qDebug()<<sqlquery.lastError();
                        }
                        return false;
                    }
                    else{
                        //删除好友列表信息
                        QString deletefl="DELETE FROM groupMember WHERE groupID = ";
                        deletefl.append(grpID);
                        if(sqlquery.prepare(deletefl)){
                            if(!sqlquery.exec(deletefl))
                            {
                                qDebug()<<sqlquery.lastError();
                            }
                            return false;
                        }
                        else{

                            return true;
                        }
                    }
                }
                return false;
            }
        }
    }
    else{
        qDebug()<<sqlquery.lastError();
        return false;
    }
}

bool Qt_chat_sql::getFriendList(int userID, QStringList &friendlist){//参数传输有疑问
    QString usrID=QString::number(userID);
    QString selectFriend="SELECT * FROM group_info WHERE type = 0 AND friendID1 = ";
    selectFriend.append(usrID);
    if(sqlquery.prepare(selectFriend)){
        if(!sqlquery.exec(selectFriend))
        {
            qDebug()<<sqlquery.lastError();
            return false;
        }
        else
        {
            while(sqlquery.next())
            {
                friendlist<<sqlquery.value(4).toString();
            }
        }
    }
    else{
        qDebug()<<sqlquery.lastError();
        return false;
    }
    QString selectFriend2="SELECT * FROM group_info WHERE type = 0 AND friendID2 = ";
    selectFriend2.append(usrID);
    if(sqlquery.prepare(selectFriend2)){
        if(!sqlquery.exec(selectFriend2))
        {
            qDebug()<<sqlquery.lastError();
            return false;
        }
        else
        {
            while(sqlquery.next())
            {
                friendlist<<sqlquery.value(3).toString();
            }
            return true;
        }
    }
    else{
        qDebug()<<sqlquery.lastError();
        return false;
    }
}

bool Qt_chat_sql::groupMembers(int groupID, int *group_members){
    QString grpID=QString::number(groupID);
    QString selectMember="SELECT * FROM groupMember WHERE groupID = ";
    selectMember.append(grpID);
    if(sqlquery.prepare(selectMember)){
        if(!sqlquery.exec(selectMember))
        {
            qDebug()<<sqlquery.lastError();
            return false;
        }
        else
        {
            while(sqlquery.next())
            {
                *group_members=sqlquery.value(1).toInt();
                group_members++;
            }
            return true;
        }
    }
    else{
        qDebug()<<sqlquery.lastError();
        return false;
    }
}

bool Qt_chat_sql::login(int userID, QString password, USER_INFO &user_info){
    QString usrID=QString::number(userID);
    QString selectpassword="SELECT * FROM user_info WHERE userID = ";
    selectpassword.append(userID);
    selectpassword.append(" AND password = ");
    selectpassword.append(password);
    if(sqlquery.prepare(selectpassword)){
        if(!sqlquery.exec(selectpassword))
        {
            qDebug()<<sqlquery.lastError();
            return false;
        }
        else
        {

            while(sqlquery.next())
            {
                user_info.userName=sqlquery.value(1).toString();
                user_info.address=sqlquery.value(3).toString();
            }
            return true;
        }
    }
    else{
        qDebug()<<sqlquery.lastError();
        return false;
    }
}

bool Qt_chat_sql::editUser_info(int userID, USER_INFO user_info){
    QString usrID=QString::number(userID);
    QString updateinfo="UPDATE user_info WHERE userID = ";
    updateinfo.append(usrID);
    updateinfo.append(" SET userName = '");
    updateinfo.append(user_info.userName);
    updateinfo.append("' , password = '");
    updateinfo.append(user_info.password);
    updateinfo.append("' , sculptureaddress = '");
    updateinfo.append(user_info.address);
    updateinfo.append("'");
    if(sqlquery.prepare(updateinfo)){
        if(!sqlquery.exec(updateinfo))
        {
            qDebug()<<sqlquery.lastError();
            return false;
        }
        else
        {
            return true;
        }
    }
    else{
        qDebug()<<sqlquery.lastError();
        return false;
    }
}

bool Qt_chat_sql::deleteMember(int groupID, int userID, int senderID,bool type){//0:删除好友 1:将别人踢出群组
    if(type==0){
        QString deletefriendList;
        QString deletefriendGroup;
        QString grpID=QString::number(groupID);

        //在好友列表中删除
        deletefriendList="DELETE FROM friendList WHERE groupID = ";
        deletefriendList.append(grpID);
        if(sqlquery.prepare(deletefriendList)){
            if(!sqlquery.exec(deletefriendList))
            {
                qDebug()<<sqlquery.lastError();
                return false;
            }
            else
            {
                return true;
            }
        }
        else{
            qDebug()<<sqlquery.lastError();
            return false;
        }

        //在群组信息中删除
        deletefriendGroup="DELETE FROM groupMember WHERE groupID = ";
        deletefriendGroup.append(grpID);

        if(sqlquery.prepare(deletefriendGroup)){
            if(!sqlquery.exec(deletefriendGroup))
            {
                qDebug()<<sqlquery.lastError();
                return false;
            }
            else
            {
                return true;
            }
        }
        else{
            qDebug()<<sqlquery.lastError();
            return false;
        }
    }
    else{
        QString deletemember;
        QString deletelist;
    }
}
