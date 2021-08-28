#include "messagefromserver.h"
using namespace MessageFromServer;
Success::Success(bool _yes):yes(_yes){

}
GrouporFriend::GrouporFriend(int _ID=0,QString _Name=""):ID(_ID),Name(_Name){

}
int GrouporFriend::getID(){
    return ID;
}
QString GrouporFriend::getName(){
    return Name;
}
