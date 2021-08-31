#include "mytcpserver.h"
MyTcpServer* MyTcpServer::tcpserverhand = nullptr;
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{

}
void MyTcpServer::openTCPserver(QString IP, QString port)
{
    qDebug()<<"IP = "<<IP<<"  port = "<<port.toInt();
    QHostAddress hostadd;
    hostadd.setAddress(IP);
    if(tcp_server.listen(hostadd,port.toInt())){
//    if(tcp_server.listen(QHostAddress::Any,port.toInt())){
        connect(&tcp_server,SIGNAL(newConnection()),this,SLOT(slot_creatNewConnection()));
    }
    else{
        qDebug()<<"server start error !";
    }
}
void MyTcpServer::slot_creatNewConnection(){
    QTcpSocket* tcp_socket = tcp_server.nextPendingConnection();
    tcp_socket0 = tcp_socket;
    connect(tcp_socket,SIGNAL(readyRead()),this,SLOT(slot_readyread()));
    qDebug()<<"someone connected!"<<tcp_socket->peerAddress();
//    onlineList.append(tcp_socket);
//    qDebug()<<"there are "<<onlineList.count()<<"user online !";
}
void MyTcpServer::slot_readyread(){
    connect(this,SIGNAL(sig_readdata(QTcpSocket*)),this,SLOT(slot_readdata(QTcpSocket*)));
    qDebug()<<"ready read.";
    emit(sig_readdata(tcp_socket0));
}
void MyTcpServer::slot_readdata(QTcpSocket* tcp_socket){
    qDebug()<<"read data";
    QByteArray databyte = tcp_socket->readAll();
    char* data=databyte.data();
    //qDebug()<<databyte;
    int *mestype=(int*) data;

    Qt_chat_sql* server2db=Qt_chat_sql::getHand();
    server2db->InitDatabase();//数据库初始化
    switch (*mestype){
    case C2S::MSG_REGISTER://注册
        {qDebug()<<"MSG_REGISTER";
        C2S::Register mes = *(C2S::Register*) (data);
        QString usrName;QString password;
        usrName = mes.name;
        qDebug()<<"name:"<<usrName;
        password = mes.password;
        qDebug()<<"password:"<<password;
        //注册到数据库中，分配usrID
        int usrID = server2db->Register(usrName,password);
        qDebug()<<"usrID:"<<usrID;
        qDebug()<<"MSG_REGISTER:"<<"usrID:"<<usrID;
        bool dbreply;
        if(usrID==0)
            dbreply = false;
        else
            dbreply = true;
        clientList.insert(usrID,tcp_socket);//usrID==0表示分配ID失败

        S2C::Register reply2client;
        reply2client.type = S2C::SERVER_REPLY;
        reply2client.success = dbreply;
        reply2client.usrID = usrID;
        if(reply2client.success)
        {   qDebug()<<"MSG_REGISTER:"<<"Reply to client:"<<"Register OK!";
            char temp[60]="Register OK!";
            memcpy(reply2client.text,temp,sizeof(temp));
        }
        else
        {   qDebug()<<"MSG_REGISTER:"<<"Reply to client:"<<"Register failed!";
            char temp[60]="Register failed!";
            memcpy(reply2client.text,temp,sizeof(temp));
        }
        tcp_socket->write((char*)&reply2client, sizeof(reply2client));
        break;}
    case C2S::MSG_LOG://登录、登出
        {qDebug()<<"MSG_LOG";
        C2S::Log mes = *(C2S::Log*) (data);
        qDebug()<<"mes.operation"<<mes.operation;
        if(mes.operation==1)//Log in
        {   struct USER_INFO userinfo;
            bool dbreply = server2db->login(mes.id,mes.password,userinfo);
            clientList.insert(mes.id,tcp_socket);
            qDebug()<<userinfo.userID<<userinfo.userName<<userinfo.password;
            S2C::Response reply2client;
            reply2client.type = S2C::SERVER_REPLY;
            reply2client.success = dbreply;
            if(reply2client.success)
            {   qDebug()<<"MSG_LOG:"<<"Reply to client:"<<"Log in OK!";
                char temp[60]="Log in OK!";
                memcpy(reply2client.text,temp,sizeof(temp));
            }
            else
            {   qDebug()<<"MSG_LOG:"<<"Reply to client:"<<"Log in failed!";
                char temp[60]="Log in failed!";
                memcpy(reply2client.text,temp,sizeof(temp));
            }
            tcp_socket->write((char*)&reply2client, sizeof(reply2client));
        }
        else//Log out
        {
            bool dbreply = server2db->Logout(mes.id,mes.sendTime);
            qDebug()<<dbreply;
            tcp_socket = NULL;
        }
        break;}
    case C2S::MSG_TEXT://接收并转发text消息
        {qDebug()<<"MSG_TEXT";
        C2S::Text mes = *(C2S::Text*) (data);
        //写入database
        MESSAGE todb;
        todb.groupID = mes.groupID;
        todb.senderID = mes.senderID;
        todb.time = mes.sendTime;
        todb.type = ".txt";
        todb.content = mes.text;
        qDebug()<<mes.sendTime<<todb.time;
        qDebug()<<mes.groupID;
        qDebug()<<todb.groupID<<todb.senderID<<todb.content;
        bool dbreply = server2db->newMsg(todb);
        qDebug()<<dbreply;

        //回复发送方发送成功
        S2C::Response reply2client2;
        reply2client2.type = S2C::SERVER_REPLY;
        reply2client2.success = dbreply;
        memset(reply2client2.text,0,sizeof(reply2client2.text));
        if(dbreply)//写入database成功
        {
            char temp[60]="Message has send";
            qDebug()<<"MSG_TEXT:"<<"Reply to sender:"<<"Message has send.";
            memcpy(reply2client2.text,temp,sizeof(temp));
        }
        else//写入database失败
        {   qDebug()<<"MSG_TEXT:"<<"Reply to sender:"<<"Message has not send.";
            char temp[60]="Message has not send";
            memcpy(reply2client2.text,temp,sizeof(temp));
        }
        tcp_socket->write((char*)&reply2client2, sizeof(reply2client2));

        qDebug()<<mes.groupID;
        //给接收的群组发消息
        if(dbreply)
        {
            S2C::Text reply2client;
            reply2client.type = S2C::SERVER_MSG_TEXT;
            reply2client.senderID = mes.senderID;
            reply2client.groupID = mes.groupID;
            reply2client.sendTime = mes.sendTime;
            memcpy(reply2client.text,mes.text,sizeof(mes.text));
            GROUP_INFO groupinfo;
            bool dbreply2 = server2db->getGroup_info(mes.groupID,groupinfo);
            qDebug()<<mes.groupID;
            qDebug()<<dbreply2<<groupinfo.groupID<<groupinfo.friendID;

            if(groupinfo.type==1)//要区分好友和群,1是群
            {
                int groupmemberid[100];//一个群最多100人
                int groupcount = server2db->groupMembers(mes.groupID,groupmemberid);
                qDebug()<<mes.groupID<<groupmemberid;
                qDebug()<<groupcount;
                for(int i=0;i<groupcount;i++)//遍历群成员
                {   if(reply2client.senderID!=groupmemberid[i])//跳过发送方本人
                    {   QHash<int,QTcpSocket*>::iterator iter;
                        iter = clientList.find(groupmemberid[i]);
                        if(iter!=clientList.end()&&iter.value()!=NULL)//在线
                        {   tcp_socket = iter.value();
                            tcp_socket->write((char*)&reply2client, sizeof(reply2client));
                        }
                        else//离线,不在这里发消息，请求消息列表时再给
                        {

                        }
                    }
                }
            }
            else if(groupinfo.type==0){
                int friendID = groupinfo.friendID;
                QHash<int,QTcpSocket*>::iterator iter;
                iter = clientList.find(friendID);
                if(iter!=clientList.end()&&iter.value()!=NULL)//在线
                {   tcp_socket = iter.value();
                    tcp_socket->write((char*)&reply2client, sizeof(reply2client));
                }
                else//离线,不在这里发消息，请求消息列表时再给
                {

                }
            }
        }
        else
            qDebug()<<"MEG_TEXT didn't write in database.";
        break;}
    case C2S::MSG_REQUEST:
        {qDebug()<<"MSG_REQUEST";
        C2S::Request mes = *(C2S::Request*) (data);
        if(mes.add)//添加好友
        {
            qDebug()<<mes.senderID<<mes.text;
            bool dbreply;
            //向B发送待确认新好友信息
            S2C::NewFriend reply2client2;
            reply2client2.type = S2C::SERVER_NEWFRIEND;
            reply2client2.senderID = mes.senderID;
//            QStringList sender;
//            sender.append((QString) mes.senderID);
//            USER_INFO* sender_infoList;
//            server2db->getUser_info(sender,sender_infoList);
//            std::string str=sender_infoList->userName.toStdString();
            QString senderName;
            senderName = server2db->getUserName(mes.senderID);
            qDebug()<<senderName;
            std::string str=senderName.toStdString();
            const char* cstr=str.c_str();
            memset(reply2client2.senderName,0,30*sizeof(char));
            memcpy(reply2client2.senderName,cstr,str.length());
            memset(reply2client2.text,0,50*sizeof(char));
            memcpy(reply2client2.text,mes.text,sizeof(mes.text));

            qDebug()<<reply2client2.type<<reply2client2.senderID<<reply2client2.senderName<<reply2client2.text;

            QHash<int,QTcpSocket*>::iterator iter;
            iter = clientList.find(mes.targetID);
            if(iter!=clientList.end()&&iter.value()!=NULL)//对方在线
            {
                QTcpSocket* tcp_socket2 = iter.value();
                tcp_socket2->write((char*)&reply2client2, sizeof(reply2client2));
                dbreply = true;
            }
            else//对方离线
            {
                dbreply = server2db->addFriendrequest(mes.senderID,mes.targetID,mes.text);
            }

            //向A发送确认收到
            S2C::Response reply2client;
            reply2client.type = S2C::SERVER_REPLY;
            reply2client.success = dbreply;//写入db申请列表
            if(dbreply)
            {
                char temp[60]="Request has send.";
                memset(reply2client.text,0,sizeof(reply2client.text));
                memcpy(reply2client.text,temp,sizeof(temp));
            }
            else
            {
                char temp[60]="Request has send.";
                memset(reply2client.text,0,sizeof(reply2client.text));
                memcpy(reply2client.text,temp,sizeof(temp));
            }
            tcp_socket->write((char*)&reply2client, sizeof(reply2client));
        }
        break;}
    case C2S::MSG_ACCEPT:
        {qDebug()<<"MSG_ACCEPT";
        C2S::Accept mes = *(C2S::Accept*)data;
        QTcpSocket* tcp_socket2;
        S2C::Response reply2client,reply2client2;
        reply2client.type = S2C::SERVER_REPLY;
        reply2client2.type = S2C::SERVER_REPLY;
        reply2client.success = mes.accept;
        reply2client2.success = mes.accept;
        qDebug()<<mes.senderID<<mes.targetID;
        if(mes.kind)//好友申请
        {
            bool dbreply = server2db->deleteFriendrequest(mes.senderID,mes.targetID);
            qDebug()<<dbreply;
            if(mes.accept)
            {
                qDebug()<<"Friend request accepted.";
                char temp[60]="Friend request accepted.";
                memcpy(reply2client.text,temp,sizeof(temp));
                memcpy(reply2client2.text,temp,sizeof(temp));
                //建立2人对话型群组
                QString qstr = QString::number(mes.targetID);
                qDebug()<<qstr<<mes.senderID;
                int groupid = server2db->newGroup(0,qstr,mes.senderID);
                qDebug()<<groupid;
            }
            else
            {
                char temp[60]="Friend request refused.";
                memcpy(reply2client.text,temp,sizeof(temp));
                memcpy(reply2client2.text,temp,sizeof(temp));
            }
        }
        else//群主批复
        {
            server2db->deleteGrouprequest(mes.senderID,mes.targetID);
            if(mes.accept)
            {
                char temp[60]="Join group request accepted.";
                memcpy(reply2client.text,temp,sizeof(temp));
                memcpy(reply2client2.text,temp,sizeof(temp));
            }
            else
            {
                char temp[60]="Join group request refused.";
                memcpy(reply2client.text,temp,sizeof(temp));
                memcpy(reply2client2.text,temp,sizeof(temp));
            }
        }
        QHash<int,QTcpSocket*>::iterator iter;
        iter = clientList.find(mes.senderID);
        if(iter!=clientList.end()&&iter.value()!=NULL)//对方不在线则不发送好友已添加或被拒绝的提示信息
        {
            tcp_socket2 = iter.value();
            tcp_socket2->write((char*)&reply2client2, sizeof(reply2client2));
        }
        tcp_socket->write((char*)&reply2client, sizeof(reply2client));
        break;
        }
    case C2S::MSG_GROUP:
        {qDebug()<<"MSG_GROUP";
        C2S::Group mes = *(C2S::Group*)data;
        if(mes.newGroup)//建群
        {
            //写入database
            qDebug()<<mes.name<<mes.userID;
            int groupID = server2db->newGroup(1,mes.name,mes.userID);
            qDebug()<<groupID;
            //回复建群成功
            S2C::NewGroup reply2client;
            reply2client.type = S2C::SERVER_NEWGROUP;
            reply2client.groupID = groupID;//groupID==0表示未成功
            memcpy(reply2client.groupName,mes.name,sizeof(mes.name));
            tcp_socket->write((char*)&reply2client, sizeof(reply2client));
        }
        else//删群
        {

        }
        break;}
    case C2S::MSG_JOIN:
        {qDebug()<<"MSG_JOIN";
        C2S::Join mes = *(C2S::Join*)data;
        if(mes.join)//加群
        {
            int masterID = server2db->getgroupOwner(mes.targetID); //根据groupID找到群主ID
            qDebug()<<mes.targetID<<masterID;

            QHash<int,QTcpSocket*>::iterator iter;
            iter = clientList.find(masterID);
            QTcpSocket* tcp_socket2 = iter.value();
            if(iter.value()!=NULL&&iter!=clientList.end())//群主在线
            {   //给群主发NewJoin
                S2C::NewJoin reply2master;
                reply2master.type = S2C::SERVER_NEWJOIN;
                reply2master.senderID = mes.senderID;
                //查找senderName
//                QStringList sender;
//                sender.append((QString) mes.senderID);
//                USER_INFO* sender_infoList;
//                server2db->getUser_info(sender,sender_infoList);
//                std::string str=sender_infoList->userName.toStdString();
                QString senderName;
                senderName = server2db->getUserName(mes.senderID);
                qDebug()<<mes.senderID<<senderName;
                std::string str=senderName.toStdString();
                const char* cstr=str.c_str();
                memset(reply2master.senderName,0,sizeof(30));
                memcpy(reply2master.senderName,cstr,str.length());
                memcpy(reply2master.text,mes.text,sizeof(mes.text));

                tcp_socket2->write((char*)&reply2master, sizeof(reply2master));
                //给请求加群的用户确认消息
                S2C::Response reply2client;
                reply2client.type = S2C::SERVER_REPLY;
                reply2client.success = true;
                char temp[60]="Join message has send.";
                memset(reply2client.text,0,sizeof(reply2client.text));
                memcpy(reply2client.text,temp,sizeof(temp));
                tcp_socket->write((char*)&reply2client, sizeof(reply2client));
            }
            else//群主离线
            {
                //写入数据库
                server2db->addGrouprequest(mes.senderID,mes.targetID,mes.text);
            }
        }
        else//退群
        {

        }
        break;}



    case C2S::MSG_FRIENDLIST:
    {qDebug()<<"MSG_FRIENDLIST";
    C2S::FriendList mes = *(C2S::FriendList*)data;
    FRIEND_LIST friendList[20];
    int dbreply;
    dbreply = server2db->getFriendList(mes.userID,friendList);
    qDebug()<<dbreply<<mes.userID<<friendList[0].groupID<<friendList[0].friendID;
    S2C::FriendList reply2client;
    reply2client.type = S2C::SERVER_FRIENDLIST;
    if(dbreply>=0&&dbreply<=20)
    {
        reply2client.success = true;
        reply2client.size = dbreply;
    }
    else
    {
        reply2client.success = false;
        reply2client.size = dbreply;
    }
    int j=0;
    for(int i=0;i<dbreply;i++){
        if(friendList[i].friendID!=0)//friendID==0是群组，不是好友
        {reply2client.friends[j].groupID = friendList[i].groupID;
        reply2client.friends[j].personID = friendList[i].friendID;
        QString friendName = server2db->getUserName(friendList[i].friendID);
        qDebug()<<friendName;
        std::string str=friendName.toStdString();
        const char* cstr=str.c_str();
        memset(reply2client.friends[j].personName,0,30*sizeof(char));
        memcpy(reply2client.friends[j].personName,cstr,str.length());
        qDebug()<<reply2client.friends[j].personName;
        j++;}
    }
    for(;j<20;j++)
    {
        reply2client.friends[j].groupID = 0;
        reply2client.friends[j].personID = 0;
        memset(reply2client.friends[j].personName,0,sizeof(30));
    }
    tcp_socket->write((char*)&reply2client, sizeof(reply2client));
    break;}

    case C2S::MSG_GROUPLIST:
    {qDebug()<<"MSG_GROUPLIST";
    C2S::GroupList mes = *(C2S::GroupList*)data;
    FRIEND_LIST groupList[20];
    S2C::GroupList reply2client;
    reply2client.type = S2C::SERVER_GROUPLIST;
    int dbreply;
    dbreply = server2db->getFriendList(mes.userID,groupList);
    if(dbreply>=0&&dbreply<=20)
    {
        reply2client.success = true;
        reply2client.size = dbreply;
    }
    else
    {
        reply2client.success = false;
        reply2client.size = dbreply;
    }
    int j=0;
    for(int i=0;i<dbreply;i++){
        if(groupList[i].friendID==0)//friendID==0是群组，不是好友
        {reply2client.groups[j].groupID = groupList[i].groupID;
        QString groupName = server2db->getGroupName(groupList[i].groupID);
        qDebug()<<groupName;
        std::string str=groupName.toStdString();
        const char* cstr=str.c_str();
        memset(reply2client.groups[j].groupName,0,30*sizeof(char));
        memcpy(reply2client.groups[j].groupName,cstr,str.length());
        qDebug()<<reply2client.groups[j].groupName;
        j++;}
    }
    for(;j<20;j++)
    {
        reply2client.groups[j].groupID = 0;
        memset(reply2client.groups[j].groupName,0,sizeof(30));
    }
    tcp_socket->write((char*)&reply2client, sizeof(reply2client));
    break;}


    case C2S::MSG_WAITINNG_FRIEND:
    {qDebug()<<"MSG_WAITINNG_FRIEND";
    C2S::WaitingFriends mes = *(C2S::WaitingFriends*)data;
    int dbreply;
    FRIEND_REQUEST frdrequest[20];
    dbreply = server2db->getFriendrequest(mes.userID,frdrequest);
    qDebug()<<mes.userID<<frdrequest[0].senderID<<frdrequest[0].recieverID<<frdrequest[0].content;
    qDebug()<<dbreply;

    S2C::NewFriendWaiting reply2client;
    reply2client.type = S2C::SERVER_WAITING_FRIEND;
    if(dbreply>=0&&dbreply<=20)
    {
        reply2client.success = true;
        reply2client.size = dbreply;
    }
    else
    {
        reply2client.success = false;
        reply2client.size = dbreply;
    }
    for(int i=0;i<dbreply;i++){
        reply2client.friends[i].personID = frdrequest[i].senderID;
        QString friendName = server2db->getUserName(frdrequest[i].senderID);
        qDebug()<<frdrequest[i].senderID<<friendName;
        std::string str=friendName.toStdString();
        const char* cstr=str.c_str();
        memset(reply2client.friends[i].personName,0,30*sizeof(char));
        memcpy(reply2client.friends[i].personName,cstr,str.length());
        QString friendIntro = frdrequest[i].content;
        std::string str2=friendIntro.toStdString();
        const char* cstr2=str2.c_str();
        memset(reply2client.friends[i].text,0,sizeof(reply2client.friends[i].text));
        memcpy(reply2client.friends[i].text,cstr2,str2.length());
    }
    tcp_socket->write((char*)&reply2client, sizeof(reply2client));
    break;}


    case C2S::MSG_WAITINNG_GROUP:
    {qDebug()<<"MSG_WAITINNG_GROUP";
    C2S::WaitingGroups mes = *(C2S::WaitingGroups*)data;
    int dbreply;
    GROUP_REQUEST grprequest[20];
    qDebug()<<mes.userID;
    dbreply = server2db->getGrouprequest(mes.userID,grprequest);
    qDebug()<<mes.userID<<grprequest[0].senderID<<grprequest[0].groupID<<grprequest[0].content;
    qDebug()<<dbreply;

    S2C::NewJoinWaiting reply2client;
    reply2client.type = S2C::SERVER_WAITING_GROUP;
    if(dbreply>=0&&dbreply<=20)
    {
        reply2client.success = true;
        reply2client.size = dbreply;
    }
    else
    {
        reply2client.success = false;
        reply2client.size = dbreply;
    }
    for(int i=0;i<dbreply;i++){
        reply2client.members[i].senderID = grprequest[i].senderID;
        reply2client.members[i].groupID = grprequest[i].groupID;
        qDebug()<<grprequest[i].groupID;
        QString friendName = server2db->getGroupName(grprequest[i].groupID);
        std::string str=friendName.toStdString();
        const char* cstr=str.c_str();
        memset(reply2client.members[i].senderName,0,30*sizeof(char));
        memcpy(reply2client.members[i].senderName,cstr,str.length());
        QString friendIntro = grprequest[i].content;
        std::string str2=friendIntro.toStdString();
        const char* cstr2=str2.c_str();
        memset(reply2client.members[i].text,0,sizeof(reply2client.members[i].text));
        memcpy(reply2client.members[i].text,cstr2,str2.length());
    }
    tcp_socket->write((char*)&reply2client, sizeof(reply2client));
    break;}



    case C2S::MSG_RECORD:
    {qDebug()<<"MSG_RECORD";
    C2S::Record mes = *(C2S::Record*)data;
    S2C::Record reply2client;
    reply2client.type = S2C::SERVER_TEXTRECORD;
    int dbreply;
    MESSAGE messagelist[10];
    long long int time = mes.sendTime;
    dbreply = server2db->get_history(10,mes.groupID,messagelist,time);
    qDebug()<<dbreply<<messagelist[0].content;
    if(dbreply>0)
    {
        reply2client.success = true;
        reply2client.messageNumber = dbreply;
        int i=0;
        for(;i<dbreply;i++)
        {
//            reply2client.history[i].time = messagelist[i].time.toTime_t();
            reply2client.history[i].time = messagelist[i].time;
            reply2client.history[i].senderID = messagelist[i].senderID;
            QString senderName = server2db->getUserName(messagelist[i].senderID);
            qstring2char(reply2client.history[i].senderName,senderName,30*sizeof(char));
            qstring2char(reply2client.history[i].content,messagelist[i].content,100*sizeof(char));
            qDebug()<<reply2client.history[i].senderName<<reply2client.history[i].content;
        }
    }
    else
    {
        reply2client.success = false;
    }
    tcp_socket->write((char*)&reply2client, sizeof(reply2client));
    break;}


    case C2S::MSG_TIME:
    {qDebug()<<"MSG_TIME";
    C2S::Time mes = *(C2S::Time*)data;
    S2C::Time reply2client;
    reply2client.type = S2C::SERVER_LATEST_MSG_TIME;
    int dbreply;
    FRIEND_LIST frgrlist[20];
    //QDateTime time = QDateTime::fromTime_t(mes.sendTime);
    qDebug()<<mes.userID;
    dbreply = server2db->getFriendList(mes.userID,frgrlist);
    qDebug()<<dbreply;
    if(dbreply>0)
    {
        reply2client.success = true;
//        QStringList groupIDList;
        GROUP_INFO groupinfo;
        bool dbreply2;
        for(int i=0;i<dbreply;i++)
        {   qDebug()<<i<<frgrlist[i].groupID<<frgrlist[i].friendID;
            if(frgrlist[i].friendID==0)
            {
                reply2client.group[i].isfriend=false;
            }
            else
            {
                reply2client.group[i].isfriend=true;
            }
            reply2client.group[i].groupID = frgrlist[i].groupID;
            dbreply2 = server2db->getGroup_info(frgrlist[i].groupID,groupinfo);
            qDebug()<<dbreply2;
            qDebug()<<mes.userID;
            time_t logouttime = server2db->getLogoutTime(mes.userID);
            qDebug()<<logouttime<<groupinfo.time;
            if(logouttime<groupinfo.time)
            {
                reply2client.group[i].ifnew=true;
                qDebug()<<i<<reply2client.group[i].ifnew;
            }
//            groupIDList.append((QString)frgrlist[i].groupID);
        }
//        //QDateTime reqTime = QDateTime::fromTime_t(mes.reqTime);
//        int groupNum;
//        GROUP_INFO groupinfo[20];
//        groupNum = server2db->getGroup_info(groupIDList,groupinfo);
//        for(int i=0;i<dbreply;i++){
//            if((uint)mes.reqTime<groupinfo->time.toTime_t())
//            {
//                reply2client.group[i].ifnew=true;
//            }
//        }
    }
    else
    {
        reply2client.success = false;
    }
    tcp_socket->write((char*)&reply2client, sizeof(reply2client));

    break;}
    }
}

void qstring2char(char* ans,QString& qstr,int size)
{
    std::string str=qstr.toStdString();
    const char* cstr=str.c_str();
    memset(ans,0,size);
    memcpy(ans,cstr,str.length());
}
