#include "messagefromserver.h"
using namespace S2C;
/*Server Reply whether your behavior succeed*/
Success::Success(bool _yes):yes(_yes){

}
bool Success::isSuccess(){
    return yes;
}
Login::Login(bool _yes):Success(_yes){

}
FriendRequest::FriendRequest(bool _yes):Success(_yes){

}
Join::Join(bool _yes):Success(_yes){

}
SetName::SetName(bool _yes):Success(_yes){

}
SetAvatar::SetAvatar(bool _yes):Success(_yes){

}
SetGroupName::SetGroupName(bool _yes,QString _groupName):Success(_yes),groupName(_groupName){

}
QString SetGroupName::getGroupName(){
    return groupName;
}
/*Server's message to client B*/
AbstractMessage::AbstractMessage(int _senderID, int _targetID, QDateTime _sendTime)
    : senderID(_senderID), targetID(_targetID), sendTime(_sendTime)
{

}
Text::Text(int _senderID, int _targetID, QDateTime _sendTime, QString _text)
    : AbstractMessage(_senderID, _targetID, _sendTime),
      _text(_text)
{

}


/*new friend*/
NewFriend::NewFriend(int _senderID,QString _senderName):senderID(_senderID),senderName(_senderName){

}
int NewFriend::getSenderID(){
    return senderID;
}
QString NewFriend::getSenderName(){
    return senderName;
}

NewFriendOK::NewFriendOK(bool _yes,QString _friendName):Success(_yes),friendName(_friendName){

}
QString NewFriendOK::getFriendName(){
    return friendName;
}

NewGroup::NewGroup(bool _yes,int _groupID,QString _groupName,bool _ifnew)
    :Success(_yes),groupID(_groupID),groupName(_groupName),ifnew(_ifnew){

}
int NewGroup::getGroupID(){
    return groupID;
}
QString NewGroup::getGroupName(){
    return groupName;
}
/*new or delete Group,newJoin,joinOK*/
NewJoin::NewJoin(int _senderID,QString _senderName):senderID(_senderID),senderName(_senderName){

}
int NewJoin::getSenderID(){
    return senderID;
}
QString NewJoin::getSenderName(){
    return senderName;
}

JoinOK::JoinOK(bool _yes,QString _groupName):Success(_yes),groupName(_groupName){

}
QString JoinOK::getGroupName(){
    return groupName;
}

