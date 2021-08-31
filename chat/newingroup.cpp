#include "newingroup.h"
#include "ui_newingroup.h"

newinGroup::newinGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newinGroup)
{
    ui->setupUi(this);
}

newinGroup::~newinGroup()
{
    delete ui;
}

void newinGroup::setinfo(QString groupid, QString groupname, QString newid, QString newname)
{
    this->ui->groupid->setText(groupid);
    this->ui->groupname->setText(groupname);
    this->ui->senderid->setText(newid);
    this->ui->sendername->setText(newname);
}

void newinGroup::on_know_clicked()
{
    this->close();
}
