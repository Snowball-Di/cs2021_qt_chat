#include "mytcpserver.h"
MyTcpServer* MyTcpServer::tcpserverhand = nullptr;
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    QHostAddress hostadd;
    hostadd.setAddress(IP);
    if(tcp_server.listen(hostadd,port.toInt())){
        connect(&tcp_server,SIGNAL(newConnection()),this,SLOT(slot_creatNewConnection()));
    }
    else{
        qDebug()<<"server start error !";
    }
}
void MyTcpServer::slot_creatNewConnection(){
    tcp_socket = tcp_server.nextPendingConnection();
    connect(tcp_socket,SIGNAL(clientMessage()),this,SLOT(slot_readdata()));

    qDebug()<<"someone connected!"<<tcp_socket->peerAddress();
    qDebug()<<"there are "<<clientList.count()<<"user online !";
}
void MyTcpServer::slot_readdata(){
    QByteArray databyte = tcp_socket->readAll();
    char* data=databyte.data();
    int type1,type2,type3,type4;
    type1 = (int)*data;
    type2 = (int)*(data+1);
    type3 = (int)*(data+2);
    type4 = (int)*(data+3);
    int mestype = type1*16777216+type2*65546+type3*256+type4;

    Qt_chat_sql* server2db=Qt_chat_sql::getHand();
    server2db->InitDatabase();
    switch (mestype){
    case C2S::MSG_REGISTER://注册
        {C2S::Register mes = *(C2S::Register*) (data);
        QString usrName;QString password;
        usrName = mes.name;
        password = mes.password;
        int usrID = server2db->Register(usrName,password);
        bool dbreply=false;
        if(usrID==0)
            dbreply = false;
        else
            dbreply = true;
        clientList.insert(usrID,tcp_socket);

        S2C::Register reply2client;
        reply2client.type = S2C::SERVER_REPLY;
        reply2client.success = true;
        reply2client.usrID = usrID;
        char temp[60]="Register OK!";
        memcpy(reply2client.text,temp,sizeof(temp));
        tcp_socket->write((char*)&reply2client, sizeof(reply2client));
        emit serverMessage();
        break;}
    case C2S::MSG_LOG://登录、登出
        {C2S::Log mes = *(C2S::Log*) (data);
        //Log in
        if(mes.operation==1)
        {struct USER_INFO userinfo;
        bool dbreply = server2db->login(mes.id,mes.password,userinfo);
        clientList.insert(userinfo.userID,tcp_socket);

        S2C::Response reply2client;
        reply2client.type = S2C::SERVER_REPLY;
        reply2client.success = dbreply;
        char temp[60]="Log in OK!";
        memcpy(reply2client.text,temp,sizeof(temp));
        tcp_socket->write((char*)&reply2client, sizeof(reply2client));
        emit serverMessage();
        }
        //还需加载好友列表、群组列表、消息记录、好友验证、群组验证

        //Log out
        else
        {

        }
        break;}
//    case C2S::MSG_TEXT://接收并转发text消息
//        {C2S::Text mes = *(C2S::Text*) (data);
//        MESSAGE todb;
//        todb.groupID = mes.groupID;
//        todb.senderID = mes.senderID;
//        todb.time = mes.sendTime;
//        todb.type = ".txt";
//        todb.content = mes.text;
//        bool dbreply = server2db->newMsg(todb);

//        S2C::Response reply2client2;
//        reply2client2.type = S2C::SERVER_REPLY;
//        reply2client2.success = dbreply;
//        memset(reply2client2.text,0,sizeof(reply2client2.text));
//        tcp_socket->write((char*)&reply2client2, sizeof(reply2client2));
//        emit serverMessage();

//        S2C::Text reply2client;
//        reply2client.type = S2C::SERVER_MSG_TEXT;
//        reply2client.senderID = mes.senderID;
//        reply2client.groupID = mes.groupID;
//        reply2client.sendTime = mes.sendTime;
//        int groupmemberid[100];
//        int groupcount = server2db->groupMembers(mes.groupID,groupmemberid);
//        if(groupcount>1)
//        {
//            for(int i=0;i<groupcount;i++)//遍历群成员
//            {   if(reply2client.senderID!=groupmemberid[i])//跳过发送方本人
//                {   QHash<int,QTcpSocket*>::iterator iter;
//                    iter = clientList.find(groupmemberid[i]);
//                    if(iter!=clientList.end())//在线
//                    {   tcp_socket = (iter.value());
//                        tcp_socket->write((char*)&reply2client, sizeof(reply2client));
//                    }
//                    else//离线
//                    {

//                    }
//                }
//            }
//            emit serverMessage();
//        }
//        else
//            qDebug()<<"MEG_TEXT groupMember wrong.";
//        break;}
//    case C2S::MSG_REQUEST:
//        {C2S::Request mes = *(C2S::Request*) (data);
//        if(mes.add)//添加好友
//        {
//            S2C::Response reply2client;
//            reply2client.type = S2C::SERVER_REPLY_LOGIN;
//            reply2client.success = dbreply;
//            memset(reply2client.text,0,sizeof(reply2client.text));
//            tcp_socket->write((char*)&reply2client, sizeof(reply2client));
//            emit serverMessage();


//            QStringList sender;
//            sender.append((QString) mes->sender());
//            USER_INFO* sender_infoList;
//            server2db->getUser_info(sender,sender_infoList);
//            S2C::NewFriend reply2client2(mes->sender(),sender_infoList->userName);
//            QHash<int,QTcpSocket*>::iterator iter;
//            iter = clientList.find(mes->target());
//            if(iter==clientList.end())//对方离线
//            {

//            }
//            else//对方在线
//            {
//                QTcpSocket* tcp_socket2 = iter.value();
//                tcp_socket2->write((char*)&reply2client2, sizeof(reply2client2));
//                tcp_socket->write((char*)&reply2client, sizeof(reply2client));
//                emit serverMessage();
//            }

//        }
//        //int groupID = server2db.newGroup(0,(QString)mes->target(),mes->sender());
//// ///////////////////先验证再成立二人群

//        break;}
//    case C2S::MSG_ACCEPT:
//        {
//            C2S::Accept* mes = (C2S::Accept*)data;
//            if(QString::compare( mes->kind(),"group",Qt::CaseSensitive))//群主批复
//            {
//            }
//            else if(QString::compare( mes->kind(),"friend",Qt::CaseSensitive))//好友申请通过
//            {
//                usrID1 = mes->sender();
//                usrID2 = mes->target();
//                USER_INFO* sender_infoList;
//                server2db->getUser_info(sender,sender_infoList);
//                S2C::NewFriendOK reply2client(true,);
//                QHash<int,QTcpSocket*>::iterator iter;
//                iter = clientList.find(mes->target());
//            }
//            break;
//        }
//    case C2S::MSG_GROUP:
//        {
//            if()//建群
//            {

//            }
//            else//删群
//            {

//            }
//        }
//    case C2S::MSG_JOIN:
//        {
//            if()//加群
//            {

//            }
//            else//退群
//            {

//            }
//        }
//    }
}

