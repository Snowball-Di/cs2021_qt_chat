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


void acceptReq::on_accept_clicked()
{
    emit this->signal_acceptReq(true);
    this->close();
}

void acceptReq::on_refuse_clicked()
{
    emit this->signal_acceptReq(false);
    this->close();
}
