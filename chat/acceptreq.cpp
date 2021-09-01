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

void acceptReq::setUi(int groupid, int senderid, QString sendername, QString textinfo)
{
    this->ui->id->setText(QString("%1").arg(senderid));
    this->ui->text->setText(textinfo);
    this->ui->sendername->setText(sendername);
    groupId = groupid;
    senderId = senderid;

    if(groupid == 0)
    {
        this->ui->type->setText("请求加你为好友");
        this->ui->grouplabel->hide();
        this->ui->groupid->hide();
        this->ui->text->setText(textinfo);
    }
    else
    {
        this->ui->type->setText("请求加入群聊");
        this->ui->grouplabel->show();
        this->ui->groupid->setText(QString("%1").arg(groupid));
        this->ui->text->setText(textinfo);

    }
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
