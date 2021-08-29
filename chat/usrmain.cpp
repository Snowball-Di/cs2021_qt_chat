#include "usrmain.h"
#include "ui_usrmain.h"

UsrMain::UsrMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrMain)
{
    ui->setupUi(this);
}

UsrMain::~UsrMain()
{
    delete ui;
}
