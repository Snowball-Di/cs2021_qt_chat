#include "moredetail.h"
#include "ui_moredetail.h"

Moredetail::Moredetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Moredetail)
{
    ui->setupUi(this);
}

Moredetail::~Moredetail()
{
    delete ui;
}
