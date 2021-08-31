#include "newfrireq.h"
#include "ui_newfrireq.h"

newFriReq::newFriReq(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newFriReq)
{
    ui->setupUi(this);
}

newFriReq::~newFriReq()
{
    delete ui;
}


void newFriReq::setinfo(QString senderid, QString sendername)
{
    this->ui->newfri_id->setText(senderid);
    this->ui->newfri_name->setText(sendername);
}

void newFriReq::on_know_clicked()
{
    this->close();
}
