#include "friendlist.h"
#include "ui_friendlist.h"

FriendList::FriendList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendList)
{
    ui->setupUi(this);
}

FriendList::~FriendList()
{
    delete ui;
}
