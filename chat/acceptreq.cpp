#include "acceptreq.h"
#include "ui_acceptreq.h"

acceptReq::acceptReq(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::acceptReq)
{
    ui->setupUi(this);
}

acceptReq::~acceptReq()
{
    delete ui;
}

void acceptReq::setUi(int groupid, int senderid, QString groupname, QString sendername, QString textinfo)
{

}

void acceptReq::setgroupid(int group)
{
    this->groupId = group;
}

void acceptReq::setsenderid(int sender)
{
    this->senderId = sender;
}

void acceptReq::on_accept_clicked()
{
    emit this->signal_acceptReq(this->senderId, this->groupId, true);
    this->close();
}

void acceptReq::on_refuse_clicked()
{
    emit this->signal_acceptReq(this->senderId, this->groupId, false);
    this->close();
}
